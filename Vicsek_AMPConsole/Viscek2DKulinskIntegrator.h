#pragma once
#include "Vicsek2DIntegrator.h"

class CViscek2DKulinskIntegrator : public CVicsek2DIntegrator
{
public:
	CViscek2DKulinskIntegrator() : CVicsek2DIntegrator() {};

	CViscek2DKulinskIntegrator(TaskData& td, float_2 domain) : CVicsek2DIntegrator(td, domain)
	{
		PopulateTaskData(td, domain, td.DataNew->size());
	};
	~CViscek2DKulinskIntegrator() {};

	virtual bool RealIntegrate(float noise) override;
protected:
	virtual std::string GetComment() override;
};

