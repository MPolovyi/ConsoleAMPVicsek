#pragma once
#include "Vicsek2DIntegrator.h"
class CVicsekStoppedTop : public CVicsek2DIntegrator
{
public:
	CVicsekStoppedTop() : CVicsek2DIntegrator() {};

	CVicsekStoppedTop(TaskData& td, SimulationStats s) : CVicsek2DIntegrator(td, s)
	{
		PopulateTaskData(td, s.DomainSize, td.DataNew->size());
	};
	~CVicsekStoppedTop() {};

	virtual bool RealIntegrate(float noise) override;
protected:
	virtual std::string GetComment() override;
};

