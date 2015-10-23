//
// Created by mpolovyi on 14/10/15.
//

#include "JsonHelpers.h"
#include "../../SimulationRoutines/CSimulationController.h"
#include <cereal/types/vector.hpp>
#include <cereal/archives/binary.hpp>
#include <sstream>

struct SaveData {
	std::vector<float> CoorsX;
	std::vector<float> CoorsY;
	std::vector<float> VelocX;
	std::vector<float> VelocY;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CoorsX, CoorsY, VelocX, VelocY); // serialize things by passing them to the archive
	}
};

void StartDataFlow(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer){
    writer.StartArray();
}

void WriteData(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, CSimulationController& simContr){
    writer.StartObject();
    {
        WriteSimParams(writer, simContr);
        writer.String("Noise");
        writer.Double(simContr.Noise);
		writer.String("Steps");
		writer.Int(simContr.GetSteps());
		writer.String("AverageVelocityDistribution");
		writer.StartArray();
        {
			auto tmp = simContr.GetVelocityDistribution();
            for(auto vel : tmp)
                writer.Double(vel);
        }
        writer.EndArray();
        writer.String("AverageDencityDistribution");
        writer.StartArray();
        {
            for (auto dens : simContr.GetDensityDistribution())
                writer.Double(dens);
        }
        writer.EndArray();
		
		std::stringstream ss;
		cereal::BinaryOutputArchive oarchive(ss);
		
		{
			writer.String("ParticleCoordinatesX");
			oarchive(simContr.GetParticleCoordinatesX());
			auto len = ss.str().length();
			auto tmp = ss.str();
			std::cout << ss.str();
			auto cstr = tmp.c_str();
			std::cout << cstr;
			writer.String(cstr, tmp.length());
			ss.clear();
		}

		{
			writer.String("ParticleCoordinatesY");
			const std::string tmp = ss.str();
			const char* cstr = tmp.c_str();
			writer.String(cstr, tmp.length());
			ss.clear();
		}

		{
			writer.String("ParticleVelocitiesX");
			oarchive(simContr.GetParticleVelocitiesX());
			const std::string tmp = ss.str();
			const char* cstr = tmp.c_str();
			writer.String(cstr, tmp.length());
			ss.clear();
		}

		{
			writer.String("ParticleVelocitiesY");
			oarchive(simContr.GetParticleVelocitiesY());
			const std::string tmp = ss.str();
			const char* cstr = tmp.c_str();
			writer.String(cstr, tmp.length());
			ss.clear();
		}
    }
    writer.EndObject();
}

void WriteSimParams(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, CSimulationController&simContr) {
    writer.String("ParticleCount");
    writer.Int(simContr.GetParticleCount());
    writer.String("MaxSteps");
    writer.Int(simContr.GetMaxSteps());
    writer.String("FirstTestSteps");
    writer.Int(simContr.GetFirstTestSteps());
    writer.String("Density");
    writer.Double(simContr.GetDensity());
    writer.String("BorderVelocity");
    writer.Double(simContr.GetBorderVelocity());
    writer.String("SystemSizeX");
    writer.Double(simContr.GetSystemSizeX());
    writer.String("SystemSizeY");
    writer.Double(simContr.GetSystemSizeY());
    writer.String("MinNoise");
    writer.Double(simContr.GetMinNoise());
    writer.String("MaxNoise");
    writer.Double(simContr.GetMaxNoise());
    writer.String("StepNoise");
    writer.Double(simContr.GetStepNoise());
}

void EndDataFlow(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer){
    writer.EndArray();
}