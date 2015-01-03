#pragma once
#include "Vicsek2DIntegrator.h"

class CViscek2DKulinskIntegrator : public CVicsek2DIntegrator
{
public:
	CViscek2DKulinskIntegrator() : CVicsek2DIntegrator() {};

	CViscek2DKulinskIntegrator(TaskData& td, SimulationStats s) : CVicsek2DIntegrator(td, s)
	{
		PopulateTaskData(td, s.DomainSize, td.DataNew->size());
	};
	~CViscek2DKulinskIntegrator() {};

	virtual bool RealIntegrate(float noise) override;
protected:
	virtual std::string GetComment() override;
};

