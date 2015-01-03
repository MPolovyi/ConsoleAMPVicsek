#pragma once
#include "Integrator.h"
#include "Vicsek2DMath.h"

class CIntegrator2D :
	public IIntegrator
{

public:
	CIntegrator2D() : IIntegrator() {};
	CIntegrator2D(TaskData& td, SimulationStats stats) : IIntegrator(td), m_Stats(stats)	{};
	~CIntegrator2D() {};

	std::vector<float> GetAverDencityOnSlicesX(int splits);
	std::vector<float_2> GetAverVeclocOnSlicesX(int splits);
	float_2 GetAverageVeloc();
	static float s_Noise;
	virtual void Init(TaskData& td, SimulationStats stats) 
	{
		m_Task = &td;
		m_Stats = stats;
	};
	std::string WriteComment();
	std::string GetParticleDataJSON();
protected:
	virtual std::string GetComment() { return ""; };
	virtual bool RealIntegrate(float noise) override
	{
		return false;
	};
	TaskData* m_Task;
	SimulationStats m_Stats;
	static const int s_TileSize = 512;
};