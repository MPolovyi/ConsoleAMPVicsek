#pragma once
#include "Vicsek2DIntegrator.h"

class CVicsekKulinsky2DIntegrator : CVicsek2DIntegrator
{
public:
	CVicsekKulinsky2DIntegrator(TaskData2D& td, float_2 domain) : CVicsek2DIntegrator(td, domain) {};

protected:
	virtual bool RealIntegrate(float noise) override;
};

