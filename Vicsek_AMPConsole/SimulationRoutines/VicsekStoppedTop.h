#pragma once
#include "Vicsek2DIntegrator.h"
class CVicsekStoppedTop : public CVicsek2DIntegrator
{
public:
	CVicsekStoppedTop(TaskData2D& td, float_2 domain) : CVicsek2DIntegrator(td, domain) {};
protected:
	virtual bool RealIntegrate(float noise) override;
};

