#pragma once
#include "Vicsek2DMath.h"
#include <string>
#include <vector>
#include "CommonStructs.h"

class CIntegrator2D
{
public:
	CIntegrator2D(TaskData2D& td, float_2 domain) {
		m_Task = &td;
		m_DomainSize = domain;};

	~CIntegrator2D() {};

	std::vector<float> GetAverDencityOnSlicesX(int splits);
	std::vector<float_2> GetAverVeclocOnSlicesX(int splits);
	float_2 GetAverageVeloc();
	static float s_Noise;
	virtual void Init(TaskData2D& td, float_2 domain)
	{
		m_Task = &td;
		m_DomainSize = domain;
	};
	std::string WriteComment();
protected:
	std::shared_ptr<TaskData2D> m_Task;
	virtual std::string GetComment() { return ""; };
	virtual bool RealIntegrate(float noise)
	{
		return false;
	};

	float_2 m_DomainSize;
	int m_IntR = 1;
	static const int s_TileSize = 512;
};