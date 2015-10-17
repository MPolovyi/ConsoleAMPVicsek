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

	const ParticlesAmp2D& particlesIn = *m_Task->DataOld;
    const float sliceHeight = domainSize.y / sliceCount;
	parallel_for_each(acc_veloc.extent, [=](index<1> pt_idx) restrict(amp) {
		float_2 pos = particlesIn.pos[pt_idx];
		float_2 vel = particlesIn.vel[pt_idx];

		int partSlice = int(pos.y / sliceHeight);
		
		if (partSlice > sliceCount)
			partSlice--;
		if (partSlice < 0)
			partSlice++;

		auto idx = index<1>(partSlice);
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

	const ParticlesAmp2D& particlesIn = *m_Task->DataOld;
	const float sliceHeight = domainSize.y / splits;
	parallel_for_each(acc_dens.extent, [=](index<1> pt_idx) restrict(amp) {
		float_2 pos = particlesIn.pos[pt_idx];
		int partSlice = int(pos.y / sliceHeight);

		if (partSlice > splits)
			partSlice--;
		if (partSlice < 0)
			partSlice++;

		auto idx = index<1>(partSlice);

		acc_count[idx] += 1;
	});
	const float sliceVolume = sliceHeight * domainSize.x;	

	parallel_for_each(acc_dens.extent, [=](index<1> idx) restrict(amp) {
		acc_dens[idx] = acc_count[idx] /  sliceVolume;
	});
	return dens;
}

float_2 CIntegrator2D::GetAverageVelocity()
{
	return MathHelpers::CountAverageVector(m_Task->DataOld->vel, m_Task->DataOld->size()).xy;
}

void CIntegrator2D::IntegrateFor(int steps, float noise) {
	Steps += steps;
	for(int i=0; i < steps; i++){
        RealIntegrate(noise);
    }
}

void CIntegrator2D::IntegrateWithAveragingFor(int steps, float noise) {
	Steps += steps;
	for(int i=0; i < steps; i++){
		RealIntegrate(noise);
		auto tmpVelocity = GetAverVelocityDistributionY(10);
		auto tmpDensity = GetAverDensityDistributionY(10);
		if (AverVelocityModuleDistribution.size() == 0)
			for (int j = 0; j < tmpVelocity.size(); j++)
				AverVelocityModuleDistribution.push_back(0);


		if (AverDensityDistribution.size() == 0)
			for (int j = 0; j < tmpDensity.size(); j++)
				AverDensityDistribution.push_back(0);
		
		for (int j = 0; j < tmpVelocity.size(); j++)
			AverVelocityModuleDistribution[j] += MathHelpers::Length(tmpVelocity[j]);
		
		for (int j = 0; j < tmpDensity.size(); j++)
			AverDensityDistribution[j] += tmpDensity[j];

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
