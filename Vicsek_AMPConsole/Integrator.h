#pragma once
#include "CommonStructs.h"


class IIntegrator
{
public:
	IIntegrator()
	{
		m_Task = NULL;
	};
	explicit IIntegrator(TaskData& td) : m_Task(&td) {};
	//IIntegrator(int size, accelerator_view view, accelerator acc);
	~IIntegrator() {};

	bool Integrate(float noise)
	{
		m_Task->Swap();
		return RealIntegrate(noise);
	}
protected:
	virtual bool RealIntegrate(float noise) { return false; };
	TaskData* m_Task;
};

