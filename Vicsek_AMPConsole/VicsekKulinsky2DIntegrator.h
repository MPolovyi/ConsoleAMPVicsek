#pragma once
#include "Vicsek2DIntegrator.h"

class CVicsekKulinsky2DIntegrator : CVicsek2DIntegrator
{
public:
	CVicsekKulinsky2DIntegrator();
	~CVicsekKulinsky2DIntegrator();
protected:
	virtual bool RealIntegrate(float noise) override;
};

