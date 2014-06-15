#pragma once
#include "Vicsek2DIntegrator.h"
class CVicsekStoppedTop : public CVicsek2DIntegrator
{
public:
	CVicsekStoppedTop() : CVicsek2DIntegrator() {};

	CVicsekStoppedTop(TaskData& td, float_2 domain) : CVicsek2DIntegrator(td, domain)
	{
		PopulateTaskData(td, domain, td.DataNew->size());
	};
	~CVicsekStoppedTop() {};

	virtual bool RealIntegrate(float noise) override;
protected:
	virtual std::string GetComment() override;
};

