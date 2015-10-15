//
// Created by mpolovyi on 14/10/15.
//
#ifndef VICSEK_AMPCONSOLE_DATASTRUCTURE_H
#define VICSEK_AMPCONSOLE_DATASTRUCTURE_H

#include "../../rapidjson/filereadstream.h"
#include "../../rapidjson/document.h"
#include <string>
#include <stdio.h>

enum BorderConditions{
    Transitional,
    Reflective,
    Kuette,
    DoubleKuette
};

struct SimulationData{
    BorderConditions BCond;
    int ParticleCount;
    int MaxSteps;
    int FirstTestSteps;
    float Density;
    float BorderVelocity;
    float SystemSizeX;
    float SystemSizeY;
    float MinNoise;
    float MaxNoise;
    float StepNoise;
    int Slices;

    SimulationData(std::string fName){
        FILE *fp = fopen(fName.c_str(), "r"); // non-Windows use "r"
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        rapidjson::Document document;
        document.ParseStream(is);

        ParticleCount = document["ParticleCount"].GetInt();
        MaxSteps = document["MaxSteps"].GetInt();
        FirstTestSteps = document["FirstTestSteps"].GetInt();
        Density = (float) document["Density"].GetDouble();
        BorderVelocity = (float) document["BorderVelocity"].GetDouble();
        SystemSizeX = (float) document["SystemSizeX"].GetDouble();
        SystemSizeY = (float) document["SystemSizeY"].GetDouble();
        MinNoise = (float) document["MinNoise"].GetDouble();
        MaxNoise = (float) document["MaxNoise"].GetDouble();
        StepNoise = (float) document["StepNoise"].GetDouble();
        Slices = document["Slices"].GetInt();
        std::string bc = document["BorderConditions"].GetString();

        if (bc == "Transitional"){
            BCond = BorderConditions::Transitional;
        }
        if (bc == "Reflective"){
            BCond = BorderConditions::Reflective;
        }
        if (bc == "Kuette"){
            BCond = BorderConditions::Kuette;
        }
        if (bc == "DoubleKuette"){
            BCond = BorderConditions::DoubleKuette;
        }
    }
};

struct StabilityCheckData{
    float Noise;
    float dispTest;
    int testStepsCount;
};

#endif //VICSEK_AMPCONSOLE_DATASTRUCTURE_H
