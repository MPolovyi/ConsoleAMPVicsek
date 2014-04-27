#pragma once
#include "CommonStructs.h"

class IIntegrator
{
public:
	explicit IIntegrator(TaskData& td) : m_Task(td) {};
	//IIntegrator(int size, accelerator_view view, accelerator acc);
	~IIntegrator();

	bool Integrate(float noise)
	{
		m_Task.Swap();
		return RealIntegrate(noise);
	}
protected:
	virtual bool RealIntegrate(float noise) { return true; };
	TaskData& m_Task;
};

