#pragma once
#include "Integrator2D.h"

class CVicsek2DIntegrator :
	public CIntegrator2D
{
public:
	CVicsek2DIntegrator() : CIntegrator2D() {};

	CVicsek2DIntegrator(TaskData& td, float_2 domain) : CIntegrator2D(td, domain)
	{
		PopulateTaskData(td, domain, td.DataNew->size());
	};
	~CVicsek2DIntegrator() {};
	
	virtual void Init(TaskData& td, float_2 domain) override;
	
protected:
	virtual bool RealIntegrate(float noise) override;
	virtual void PopulateTaskData(TaskData& td, float_2 domain, int partCount);

};

