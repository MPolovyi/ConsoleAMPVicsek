#include "IntegratorCollection.h"


//CIntegratorCollection::CIntegratorCollection(std::vector<TaskData*> tds, float_2 domain)
//{
//	m_Integrators = *std::make_unique<std::vector<CIntegrator2D>>(tds.size());
//	for (int i = 0; i < tds.size(); i++)
//	{
//		m_Integrators.push_back(CVicsek2DIntegrator(*tds[i], domain));
//	}
//}

CIntegratorCollection::CIntegratorCollection(std::vector<TaskData*> tds, float_2 domain, std::vector<std::shared_ptr<CIntegrator2D>> integrators) : m_Integrators(integrators)
{
	for (int i = 0; i < tds.size(); i++)
	{
		m_Integrators[i]->Init(*tds[i], domain);
	}
}

bool CIntegratorCollection::Integrate(float noise)
{
	for (int i = 0; i < m_Integrators.size(); i++)
	{
		m_Integrators[i]->Integrate(noise);
	}
	return true;
}

float CIntegratorCollection::GetAnsambleAveragedABSVeloc()
{
	auto vel = GetAverageVeloc();
	
	std::vector<float> velAbs(vel.size());
	for (int i = 0; i < vel.size(); i++)
	{
		velAbs[i] = MathHelpers::Length(vel[i]);
	}

	return MathHelpers::CountAverageVector(velAbs);
}

std::vector<float_2> CIntegratorCollection::GetAverageVeloc()
{
	std::vector<float_2> ret;
	for (int i = 0; i < m_Integrators.size(); i++)
	{
		ret.push_back(m_Integrators[i]->GetAverageVeloc());
	}
	return ret;
}

std::vector<float> CIntegratorCollection::GetAnsambleAveragedVeclocOnSplitsX(int splits)
{
	std::vector<float> ret;
	auto tmp = GetAverVeclocOnSplitsX(splits);
	for (int i = 0; i < splits; i++)
	{
		std::vector<float> tmpAbs(tmp[i].size());
		for (int j = 0; j < tmp[i].size(); j++)
		{
			tmpAbs[j] = MathHelpers::Length(tmp[i][j]);
		}
		ret.push_back(MathHelpers::CountAverageVector(tmpAbs));
	}
	return ret;
}

std::vector<std::vector<float_2>> CIntegratorCollection::GetAverVeclocOnSplitsX(int splits)
{
	std::vector<std::vector<float_2>> ret;
	for (int i = 0; i < m_Integrators.size(); i++)
	{
		ret.push_back(m_Integrators[i]->GetAverVeclocOnSplitsX(splits));
	}
	return ret;
}

CIntegrator2D& CIntegratorCollection::operator[] (int idx)
{
	return *m_Integrators[idx];
}

CIntegratorCollection::~CIntegratorCollection()
{}
