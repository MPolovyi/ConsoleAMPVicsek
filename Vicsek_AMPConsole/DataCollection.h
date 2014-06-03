#pragma once
#include <vector>
#include <amp_short_vectors.h>
#include <fstream>

using std::vector;
using std::pair;
class CDataCollection
{

protected:
	vector<pair<float, float>> m_AverSpd;
	vector<pair<vector<float>, float>> m_SliceXAverSpd;

public:
	vector<float> GetAverSpeed()
	{
		vector<float> tmp(m_AverSpd.size());
		int i = 0;
		for (auto a : m_AverSpd)
		{
			tmp[i] = a.first;
		}
		return tmp;
	};
	void AddAverSpeed(float AverSpd, float noise)
	{
		m_AverSpd.push_back(std::make_pair(AverSpd, noise));
	};
	vector<vector<float>> GetAverSpeedOnSlices()
	{
		vector<vector<float>> tmp;
		int i = 0;
		for (auto a : m_SliceXAverSpd)
		{
			tmp[i] = a.first;
		}
		return tmp;
	};
	void AddAverSpeedOnSlices(vector<float> SliceAverspd, float noise)
	{
		m_SliceXAverSpd.push_back(std::make_pair(SliceAverspd, noise));
	};

	void WriteOnDisk(char* SpeedFileName, char* SlicesFileName, std::string Comment)
	{
		std::fstream file;

		file.open(SpeedFileName, std::ios::app);
		file << Comment << std::endl << std::endl;
		for (auto a : m_AverSpd)
		{
			file << "Velocity = " << a.first << " Noise = " << a.second << std::endl;
		}
		file.close();

		file.open(SlicesFileName, std::ios::app);
		file << Comment << std::endl << std::endl;
		for (int i = 0; i < m_SliceXAverSpd[0].first.size(); i++)
		{
			for (int j = 0; j < m_SliceXAverSpd.size(); j++)
			{
				file << m_SliceXAverSpd[j].first[i] << "   ";
			}
			file << " Noise = " << m_SliceXAverSpd[i].second;
			file << std::endl;
		}
		file.close();
	};

	CDataCollection() {};
	~CDataCollection() {};
};

