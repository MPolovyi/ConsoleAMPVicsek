#pragma once
#include "Integrator.h"
#include "MathHelper.h"
#include "Vicsek2DMath.h"


class CVicsek2DIntegrator :
	public IIntegrator
{
public:
	CVicsek2DIntegrator(TaskData& td, float_2 domain) : IIntegrator(td), m_DomainSize(domain) 
	{
		PopulateTaskData(td, domain, td.DataNew->size());
	};
	~CVicsek2DIntegrator();

	std::vector<float_2> GetAverVeclocOnSplitsX(int splits);
	float_2 GetAverageVeloc();
	static float s_Noise;
protected:
	virtual bool RealIntegrate(float noise) override;

	static const int s_TileSize = 256;
	

	int m_IntR = 1;
	float_2 m_DomainSize;

private:
	void PopulateTaskData(TaskData& td, float_2 domain, int partCount);
};

