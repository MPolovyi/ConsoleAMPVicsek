//
// Created by mpolovyi on 14/10/15.
//

#include "JsonHelpers.h"
#include "../../SimulationRoutines/CSimulationController.h"

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
		writer.String("ParticleCoordinatesX");
		writer.StartArray(); 
		{
			for (auto x : simContr.GetParticleCoordinatesX())
				writer.Double(x);
		}
		writer.EndArray();
		writer.String("ParticleCoordinatesY");
		writer.StartArray();
		{
			for (auto x : simContr.GetParticleCoordinatesY())
				writer.Double(x);
		}
		writer.EndArray();
		writer.String("ParticleVelocitiesX");
		writer.StartArray();
		{
			for (auto x : simContr.GetParticleVelocitiesX())
				writer.Double(x);
		}
		writer.EndArray();
		writer.String("ParticleVelocitiesY");
		writer.StartArray();
		{
			for (auto x : simContr.GetParticleVelocitiesY())
				writer.Double(x);
		}
		writer.EndArray();

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