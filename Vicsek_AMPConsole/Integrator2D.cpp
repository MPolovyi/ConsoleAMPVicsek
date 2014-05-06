#include "Integrator2D.h"
#include "Rand/amp_tinymt_rng.h"

//Splits computation domain on <splits> areas, parallel to X axis, and computes average velocity on each.
std::vector<float_2> CIntegrator2D::GetAverVeclocOnSplitsX(int splits)
{
	std::vector<float_2> veloc(splits);
	std::vector<int> counts(splits);
	array_view<float_2, 1> acc_veloc(veloc);
	array_view<int, 1> acc_count(counts);
	int numParticles = m_Task->DataOld->size();
	extent<1> computeDomain(numParticles);

	const float_2 domainSize = m_DomainSize;
	//initialization of random generator;

	const ParticlesAmp& particlesIn = *m_Task->DataOld;

	parallel_for_each(computeDomain.tile<s_TileSize>(), [=](tiled_index<s_TileSize> ti) restrict(amp) {

		tile_static float_3 tilePosMemory[s_TileSize];
		tile_static float_3 tileVelMemory[s_TileSize];

		int idxGlobal = ti.global[0];

		float_2 pos = particlesIn.pos[idxGlobal].xy;
		float_2 vel = particlesIn.vel[idxGlobal].xy;
		float_2 acc = 0.0f;
		auto idx = index<1>((int)(domainSize.y / pos.y));
		acc_veloc[idx] += vel;
		acc_count[idx] += 1;
	});

	parallel_for_each(acc_veloc.extent, [=](index<1> idx) restrict(amp) {
		acc_veloc[idx] /= (acc_count[idx] > 0 ? acc_count[idx] : 1);
	});
	return veloc;
}

float_2 CIntegrator2D::GetAverageVeloc()
{
	return MathHelpers::CountAverageVector(m_Task->DataOld->vel, m_Task->DataOld->size()).xy;
}