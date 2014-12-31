#pragma once

#include "Vicsek2DIntegrator.h"


class CVicsek2DChepizhkoIntegrator : public CVicsek2DIntegrator
{
public:
	CVicsek2DChepizhkoIntegrator() : CVicsek2DIntegrator() {};

	CVicsek2DChepizhkoIntegrator(TaskData& td, float_2 domain) : CVicsek2DIntegrator(td, domain)
	{
		PopulateTaskData(td, domain, td.DataNew->size());
	};
	~CVicsek2DChepizhkoIntegrator() {};

	virtual bool RealIntegrate(float noise) override;
protected:
	virtual std::string GetComment() override;
};

