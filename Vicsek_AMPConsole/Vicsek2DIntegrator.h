#pragma once
#include "Integrator2D.h"
#include "Rand\amp_tinymt_rng.h"
#include "cvmarkersobj.h"

class CVicsek2DIntegrator :
	public CIntegrator2D
{
public:
	CVicsek2DIntegrator() : CIntegrator2D() {};

	CVicsek2DIntegrator(TaskData& td, SimulationStats stats) : CIntegrator2D(td, stats)
	{
		PopulateTaskData(td, stats.DomainSize, td.DataNew->size());
	};
	~CVicsek2DIntegrator() {};
	
	virtual void Init(TaskData& td, SimulationStats stats) override;
protected:
	virtual std::string GetComment() override;
	virtual bool RealIntegrate(float noise) override;
	virtual void PopulateTaskData(TaskData& td, float_2 domain, int partCount);

	tinymt_collection<1> m_Rnd = tinymt_collection<1>(extent<1>(1), std::rand());
};

