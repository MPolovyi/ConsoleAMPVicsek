#pragma once
#include "Vicsek2DIntegrator.h"


class CIntegratorCollection
{
protected:
	std::vector<CVicsek2DIntegrator> m_Integrators;

public:
	CVicsek2DIntegrator& operator[](int idx);

	CIntegratorCollection(std::vector<TaskData*> tds, float_2 domain) ;

	bool Integrate(float noise);

	//TODO: add uniform usage for 2D and 3D classes?
	float GetAnsambleAveragedABSVeloc();
	std::vector<float_2> GetAverageVeloc();

	std::vector<float> GetAnsambleAveragedVeclocOnSplitsX(int splits);
	std::vector<std::vector<float_2>> GetAverVeclocOnSplitsX(int splits);

	~CIntegratorCollection();
};

