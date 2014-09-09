#include "IntegratorCollection.h"
#include <ppl.h>
#include <concurrent_vector.h>
#include <sstream>
#include "cvmarkersobj.h"
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
	concurrency::parallel_for(size_t(0), m_Integrators.size(), [&](size_t i) {
		m_Integrators[i]->Integrate(noise);
	});
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

	return std::accumulate(velAbs.begin(), velAbs.end(), 0.0f)/velAbs.size();
}

std::vector<float_2> CIntegratorCollection::GetAverageVeloc()
{
	std::vector<float_2> ret(m_Integrators.size());
	concurrency::parallel_for(size_t(0), m_Integrators.size(), [&](size_t i) {
		ret[i] = m_Integrators[i]->GetAverageVeloc();
	});
	return ret;
}

std::vector<float> CIntegratorCollection::GetAnsambleAveragedVeclocOnSlicesX(int splits)
{
	std::vector<float> ret(splits);
	std::vector<std::vector<float_2>> tmp = GetAverVeclocOnSlicesX(splits);

	for (int i = 0; i < splits; i++)
	{
		auto si = tmp.size();
		for (int j = 0; j < si; j++)
		{
			ret[i] += MathHelpers::Length(tmp[j][i]);
		}
		ret[i] /= si;
	}

	return ret;
}

std::vector<std::vector<float>> CIntegratorCollection::GetAverDencityOnSlicesX(int splits)
{
	std::vector<std::vector<float>> ret(m_Integrators.size());
	concurrency::parallel_for(size_t(0), m_Integrators.size(), [&](size_t i) {
		ret[i] = m_Integrators[i]->GetAverDencityOnSlicesX(splits);
	});
	return ret;
}

std::vector<float> CIntegratorCollection::GetAnsambleAveragedDencityOnSlicesX(int splits)
{
	std::vector<float> ret(splits);
	std::vector<std::vector<float>> tmp = GetAverDencityOnSlicesX(splits);

	for (int i = 0; i < splits; i++)
	{
		auto si = tmp.size();
		for (int j = 0; j < si; j++)
		{
			ret[i] += tmp[j][i];
		}
		ret[i] /= si;
	}
	return ret;
}

std::vector<std::vector<float_2>> CIntegratorCollection::GetAverVeclocOnSlicesX(int splits)
{
	std::vector<std::vector<float_2>> ret(m_Integrators.size());
	concurrency::parallel_for(size_t(0), m_Integrators.size(), [&](size_t i) {
		ret[i] = m_Integrators[i]->GetAverVeclocOnSlicesX(splits);
	});
	return ret;
}

std::string CIntegratorCollection::WriteComment(std::string str)
{
	std::ostringstream oss;
	oss << "Averaging on " << m_Integrators.size() << m_Integrators[0]->WriteComment();
	return oss.str();
}

CIntegrator2D& CIntegratorCollection::operator[] (int idx)
{
	return *m_Integrators[idx];
}

CIntegratorCollection::~CIntegratorCollection()
{}
