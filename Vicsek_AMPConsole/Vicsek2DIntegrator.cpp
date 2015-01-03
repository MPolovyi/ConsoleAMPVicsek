#include "Vicsek2DIntegrator.h"

#include <fstream>


void CVicsek2DIntegrator::Init(TaskData& td, SimulationStats stats)
{
	CIntegrator2D::Init(td, stats);
	PopulateTaskData(td, stats.DomainSize, td.DataNew->size());
};

void CVicsek2DIntegrator::PopulateTaskData(TaskData& td, float_2 domain, int partCount)
{
	index<1> begin(0);
	extent<1> end(partCount);

	std::vector<float_3> posit(partCount);
	std::vector<float_3> veloc(partCount);

	array_view<float_3, 1> pos(posit);
	array_view<float_3, 1> vel(veloc);

	m_Rnd = tinymt_collection<1>(extent<1>(partCount), std::rand());

	const tinymt_collection<1>& rnd = this->m_Rnd;
	concurrency::parallel_for_each(pos.extent, [=](index<1> idx) restrict(amp) {

		pos[idx].x = rnd[idx].next_single() * domain.x;
		pos[idx].y = rnd[idx].next_single() * domain.y;
		pos[idx].z = 0;

		vel[idx].x = rnd[idx].next_single() - 0.5;
		vel[idx].y = rnd[idx].next_single() - 0.5;
		vel[idx].z = 0;

		//normalize speed
		vel[idx] *= concurrency::fast_math::rsqrt(MathHelpers::SqrLength(vel[idx].xy));
	});

	array_view<float_3, 1> posView = td.DataOld->pos.section(index<1>(begin), extent<1>(end));
	copy(pos, posView);
	array_view<float_3, 1> velView = td.DataOld->vel.section(index<1>(begin), extent<1>(end));
	copy(vel, velView);

	auto particlesOut = *td.DataOld;

	//Swap becouse we swap data before fist Integration.
	td.Swap();
}

bool CVicsek2DIntegrator::RealIntegrate(float noise)
{
	int numParticles = m_Task->DataNew->size();
	extent<1> computeDomain(numParticles);
	const int numTiles = numParticles / s_TileSize;
	const float doubleIntR = 2 * m_Stats.InterractionRadius;
	const float dampingFactor = 0.9995f;
	const float deltaTime = 0.1;

	const float_2 domainSize = m_Stats.DomainSize;
	const float intR2 = m_Stats.InterractionRadius*m_Stats.InterractionRadius;
	//initialization of random generator;
	const ParticlesAmp& particlesIn = *m_Task->DataOld;
	const ParticlesAmp& particlesOut = *m_Task->DataNew;
	const tinymt_collection<1>& rnd = this->m_Rnd;

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
		
		Vicsek2DMath::BorderCheckTransitional(pos, vel, domainSize);

		particlesOut.pos[idxGlobal].xy = pos;
		particlesOut.vel[idxGlobal].xy = vel;
	});
	return true;
}

std::string CVicsek2DIntegrator::GetComment()
{
	return "Standard Vicsek aldorithm, transitional borders";
}