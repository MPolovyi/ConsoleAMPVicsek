#pragma once
#include "stdafx.h"

class CDataCollectionGPU
{
public:
	CDataCollectionGPU();
	~CDataCollectionGPU();

protected:
	std::vector<float> m_Noise;
	std::vector<float> m_AverVel;
	std::vector<std::vector<float>> m_AverVelOnSplits;
	
};

