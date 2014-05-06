#pragma once
#include "Integrator.h"
#include "Vicsek2DMath.h"

class CIntegrator2D :
	public IIntegrator
{

public:
	CIntegrator2D() : IIntegrator() {};
	CIntegrator2D(TaskData& td, float_2 domain) : IIntegrator(td), m_DomainSize(domain)	{};
	~CIntegrator2D() {};

	std::vector<float_2> GetAverVeclocOnSplitsX(int splits);
	float_2 GetAverageVeloc();
	static float s_Noise;
	virtual void Init(TaskData& td, float_2 domain) 
	{
		m_Task = &td;
		m_DomainSize = domain;
	};
	
protected:
	virtual bool RealIntegrate(float noise) override
	{
		return false;
	};

	float_2 m_DomainSize;
	int m_IntR = 1;
	static const int s_TileSize = 256;
};