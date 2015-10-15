#include "Integrator2D.h"
#include "../Rand/amp_tinymt_rng.h"
#include <sstream>
#include "../Helpers/MathHelpers/MathHelper.h"


//Splits computation domain on <splits> areas, parallel to X axis, and computes average velocity on each.
std::vector<float_2> CIntegrator2D::GetAverVelocityDistributionY(int sliceCount)
{
	std::vector<float_2> veloc(sliceCount);
	std::vector<int> counts(sliceCount);
	array_view<float_2, 1> acc_veloc(veloc);
	array_view<int, 1> acc_count(counts);
	int numParticles = m_Task->DataOld->size();
	extent<1> computeDomain(numParticles);

	const float_2 domainSize = m_DomainSize;
	//initialization of random generator;

	const ParticlesAmp& particlesIn = *m_Task->DataOld;

	parallel_for_each(computeDomain.tile<s_TileSize>(), [=](tiled_index<s_TileSize> ti) restrict(amp) {

		tile_static float_2 tilePosMemory[s_TileSize];
		tile_static float_2 tileVelMemory[s_TileSize];

		int idxGlobal = ti.global[0];

		float_2 pos = particlesIn.pos[idxGlobal];
		float_2 vel = particlesIn.vel[idxGlobal];
		auto idx = index<1>((int)(domainSize.y / pos.y));
		acc_veloc[idx] += vel;
		acc_count[idx] += 1;
	});

	parallel_for_each(acc_veloc.extent, [=](index<1> idx) restrict(amp) {
		acc_veloc[idx] /= (acc_count[idx] > 0 ? acc_count[idx] : 1);
	});
	return veloc;
}

std::vector<float> CIntegrator2D::GetAverDensityDistributionY(int splits)
{
	std::vector<float> dens(splits);
	std::vector<int> counts(splits);
	array_view<float, 1> acc_dens(dens);
	array_view<int, 1> acc_count(counts);
	int numParticles = m_Task->DataOld->size();
	extent<1> computeDomain(numParticles);

	const float_2 domainSize = m_DomainSize;
	//initialization of random generator;

	const ParticlesAmp& particlesIn = *m_Task->DataOld;

	parallel_for_each(computeDomain.tile<s_TileSize>(), [=](tiled_index<s_TileSize> ti) restrict(amp) {

		tile_static float_3 tilePosMemory[s_TileSize];
		
		int idxGlobal = ti.global[0];

		float_2 pos = particlesIn.pos[idxGlobal];
		auto idx = index<1>((int)(domainSize.y / pos.y));
		acc_count[idx] += 1;
	});
	const float sliceVolume = (domainSize.y / splits) * domainSize.x;
	parallel_for_each(acc_dens.extent, [=](index<1> idx) restrict(amp) {
		acc_dens[idx] = acc_count[idx] /  sliceVolume;
	});
	return dens;
}

std::string CIntegrator2D::WriteComment()
{
	std::ostringstream oss;
	oss << "; Particle count = " << m_Task->DataNew->size() << "; Domain size = " << m_DomainSize.x << " " << GetComment();
	return oss.str();
}

float_2 CIntegrator2D::GetAverageVelocity()
{
	return MathHelpers::CountAverageVector(m_Task->DataOld->vel, m_Task->DataOld->size()).xy;
}

void CIntegrator2D::IntegrateFor(int steps, float noise) {
	for(int i=0; i < steps; i++){
        RealIntegrate(noise);
    }
}

void CIntegrator2D::IntegrateWithAveragingFor(int steps, float noise) {
	for(int i=0; i < steps; i++){
		RealIntegrate(noise);
		auto tmp = GetAverVelocityDistributionY(10);

		for (int j = 0; j < tmp.size(); j++){
			AverVelocityModuleDistribution[j] += MathHelpers::Length(tmp[j]);
		}
	}
    for (int i = 0; i < AverVelocityModuleDistribution.size(); i++){
        AverVelocityModuleDistribution[i] /= steps;
    }
}

int CIntegrator2D::PtCount() {
	return m_Task->DataNew->size();
}

void CIntegrator2D::ResetSteps() {
    Steps = 0;
}
