#pragma once
#include <vector>
#include <amp_short_vectors.h>
#include <fstream>

#include "../rapidjson/include/rapidjson/prettywriter.h"
#include "../rapidjson/include/rapidjson/filewritestream.h"
#include <cstdio>

using std::vector;
using std::pair;
class CDataCollection
{
protected:
	vector<pair<float, float>> m_AverSpd;
	vector<pair<vector<float>, float>> m_SliceXAverSpd;
	vector<pair<vector<float>, float>> m_SliceXAverRho;
	
	vector<pair<vector<float>, int>> m_MultyCollData;

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

	void AddAverSpeedOnSlices(vector<float> SliceAverSpd, float noise)
	{
		m_SliceXAverSpd.push_back(std::make_pair(SliceAverSpd, noise));
	};

	void AddAverRhoOnSlices(vector<float> SliceAverRho, float noise)
	{
		m_SliceXAverRho.push_back(std::make_pair(SliceAverRho, noise));
	};

	void WriteOnDisk(char* SpeedFileName, char* SlicesFileName, std::string Comment)
	{
		std::fstream file;
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> wr(s);

		wr.StartObject();

			wr.String("Comment");
			wr.String(Comment.c_str());

			wr.String("Velocity_vs_Noise");
			wr.StartObject();
				wr.String("Velocity");
				wr.StartArray();
				for (auto a : m_AverSpd)
				{
					wr.Double(a.first);
				}
				wr.EndArray();

				wr.String("Noise");
				wr.StartArray();
				for (auto a : m_AverSpd)
				{
					wr.Double(a.second);
				}
				wr.EndArray();
			wr.EndObject();
		wr.EndObject();

		file.open(SpeedFileName, std::ios::app);
		file << s.GetString();
		file.close();

		s.Clear();

		wr.StartObject();

		wr.String("Comment");
		wr.String(Comment.c_str());
		wr.String("Data");

		wr.StartArray();
		for (int i = 0; i < m_SliceXAverSpd[0].first.size(); i++)
		{
			wr.StartObject();
			wr.String("Noise");
			wr.Double(m_SliceXAverSpd[i].second);
			wr.String("Velocity_vs_Height");

			wr.StartObject();
			wr.String("Velocity");
			wr.StartArray();
			for (int j = 0; j < m_SliceXAverSpd.size(); j++)
			{
				wr.Double(m_SliceXAverSpd[j].first[i]);
			}
			wr.EndArray();

			wr.String("Height");
			wr.StartArray();
			for (int j = 0; j < m_SliceXAverSpd.size(); j++)
			{
				wr.Double(m_SliceXAverSpd[i].second);
			}
			wr.EndArray();
			wr.EndObject();
			wr.EndObject();
		}
		wr.EndArray();
		wr.EndObject();

		file.open(SlicesFileName, std::ios::app);
		file << s.GetString();
		file.close();
	};

	void WriteOnDisk(char* SpeedFileName, char* SlicesFileName, char* SlicesRhoFilename, std::string Comment)
	{
		WriteOnDisk(SpeedFileName, SlicesFileName, Comment);
		
		std::fstream file;
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> wr(s);

		wr.StartObject();

		wr.String("Comment");
		wr.String(Comment.c_str());
		wr.String("Data");

		wr.StartArray();
		for (int i = 0; i < m_SliceXAverRho[0].first.size(); i++)
		{
			wr.StartObject();
			wr.String("Noise");
			wr.Double(m_SliceXAverSpd[i].second);
			wr.String("Dencity_vs_Height");

			wr.StartObject();
			wr.String("Dencity");
			wr.StartArray();
			for (int j = 0; j < m_SliceXAverRho.size(); j++)
			{
				wr.Double(m_SliceXAverRho[j].first[i]);
			}
			wr.EndArray();

			wr.String("Height");
			wr.StartArray();
			for (int j = 0; j < m_SliceXAverRho.size(); j++)
			{
				wr.Double(m_SliceXAverRho[i].second);
			}
			wr.EndArray();
			wr.EndObject();
			wr.EndObject();
		}
		wr.EndArray();
		wr.EndObject();

		file.open(SlicesRhoFilename, std::ios::app);
		file << s.GetString();
		file.close();
	};

	CDataCollection() {};
	~CDataCollection() {};
};
