#pragma once
#include "Integrator2D.h"
#include "../Rand/amp_tinymt_rng.h"
#include <string>


class CVicsek2DIntegrator :
	public CIntegrator2D
{
public:
	CVicsek2DIntegrator(TaskData2D& td, float_2 domain) : CIntegrator2D(td, domain)
	{
		PopulateTaskData(td, domain, td.DataNew->size());
	};
	~CVicsek2DIntegrator() {};
	
	virtual void Init(TaskData2D& td, float_2 domain) override;
protected:
	virtual std::string GetComment() override;
	virtual bool RealIntegrate(float noise) override;
	virtual void PopulateTaskData(TaskData2D& td, float_2 domain, int partCount);

	tinymt_collection<1> m_Rnd = tinymt_collection<1>(extent<1>(1), std::rand());
};

