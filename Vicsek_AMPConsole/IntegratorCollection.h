#pragma once
#include "Vicsek2DIntegrator.h"


class CIntegratorCollection
{
protected:
	std::vector<std::shared_ptr<CIntegrator2D>> m_Integrators;
	
public:
	CIntegrator2D& operator[](int idx);
	CIntegratorCollection(std::vector<TaskData*> tds, float_2 domain, std::vector<std::shared_ptr<CIntegrator2D>> integrators);
	//CIntegratorCollection(std::vector<TaskData*> tds, float_2 domain);

	bool Integrate(float noise);

	//TODO: add uniform usage for 2D and 3D classes?
	float GetAnsambleAveragedABSVeloc();
	std::vector<float_2> GetAverageVeloc();

	std::vector<float> GetAnsambleAveragedVeclocOnSplitsX(int splits);
	std::vector<std::vector<float_2>> GetAverVeclocOnSplitsX(int splits);

	~CIntegratorCollection();
};
