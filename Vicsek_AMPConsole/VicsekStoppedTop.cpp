#include "VicsekStoppedTop.h"


bool CVicsekStoppedTop::RealIntegrate(float noise)
{
	int numParticles = m_Task->DataNew->size();
	extent<1> computeDomain(numParticles);
	const int numTiles = numParticles / s_TileSize;
	const float doubleIntR = m_IntR;
	const float dampingFactor = 0.9995f;
	const float deltaTime = 0.1;

	const float_2 domainSize = m_DomainSize;
	const float intR2 = m_IntR*m_IntR;
	//initialization of random generator;

	tinymt_collection<1> rnd(computeDomain, std::rand());

	const ParticlesAmp& particlesIn = *m_Task->DataOld;
	const ParticlesAmp& particlesOut = *m_Task->DataNew;

	concurrency::parallel_for_each(computeDomain.tile<s_TileSize>(), [=](tiled_index<s_TileSize> ti) restrict(amp) {

		tile_static float_3 tilePosMemory[s_TileSize];
		tile_static float_3 tileVelMemory[s_TileSize];

		const int idxLocal = ti.local[0];
		int idxGlobal = ti.global[0];

		float_2 pos = particlesIn.pos[idxGlobal].xy;
		float_2 vel = particlesIn.vel[idxGlobal].xy;
		float_2 acc = 0.0f;

		// Update current Particle using all other particles
		int particleIdx = idxLocal;
		for (int tile = 0; tile < numTiles; tile++, particleIdx += s_TileSize)
		{
			// Cache current particle into shared memory to increase IO efficiency
			tilePosMemory[idxLocal] = particlesIn.pos[particleIdx];
			tileVelMemory[idxLocal] = particlesIn.vel[particleIdx];
			// Wait for caching on all threads in the tile to complete before calculation uses the data.
			ti.barrier.wait();

			// Unroll size should be multile of m_tileSize
			// Unrolling 4 helps improve perf on both ATI and nVidia cards
			// 4 is the sweet spot - increasing further adds no perf improvement while decreasing reduces perf
			for (int j = 0; j < s_TileSize;)
			{
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++].xy, pos, tilePosMemory[j++].xy, doubleIntR, intR2, domainSize);
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++].xy, pos, tilePosMemory[j++].xy, doubleIntR, intR2, domainSize);
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++].xy, pos, tilePosMemory[j++].xy, doubleIntR, intR2, domainSize);
				Vicsek2DMath::BodyBodyInteraction(vel, tileVelMemory[j++].xy, pos, tilePosMemory[j++].xy, doubleIntR, intR2, domainSize);
			}

			// Wait for all threads to finish reading tile memory before allowing a new tile to start.
			ti.barrier.wait();
		}

		MathHelpers::RotateVector2D(vel, noise * (0.5 - rnd[ti.local].next_single()));

		vel *= dampingFactor;
		MathHelpers::NormalizeVector(vel);

		pos += vel * deltaTime;

		Vicsek2DMath::BorderCheckStoppedBottomMovingTopY(pos, vel, domainSize, rnd[ti.local].next_single(), rnd[ti.local].next_single());

		particlesOut.pos[idxGlobal].xy = pos;
		particlesOut.vel[idxGlobal].xy = vel;
	});
	return true;
}

std::string CVicsekStoppedTop::GetComment()
{
	return "Top moves 1, Bottom slovers particles in x direction";
}