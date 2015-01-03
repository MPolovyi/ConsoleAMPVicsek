#pragma once

#include "Vicsek2DIntegrator.h"


class CVicsek2DChepizhkoIntegrator : public CVicsek2DIntegrator
{
public:
	CVicsek2DChepizhkoIntegrator() : CVicsek2DIntegrator() {};

	CVicsek2DChepizhkoIntegrator(TaskData& td, SimulationStats s) : CVicsek2DIntegrator(td, s)
	{
		PopulateTaskData(td, s.DomainSize, td.DataNew->size());
	};
	~CVicsek2DChepizhkoIntegrator() {};

	virtual bool RealIntegrate(float noise) override;
protected:
	virtual std::string GetComment() override;
};

