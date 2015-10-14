//
// Created by mpolovyi on 14/10/15.
//
#ifndef VICSEK_AMPCONSOLE_DATASTRUCTURE_H
#define VICSEK_AMPCONSOLE_DATASTRUCTURE_H

#include "../../rapidjson/filereadstream.h"
#include "../../rapidjson/document.h"
#include <string>
#include <stdio.h>

struct SimulationData{
    int ParticleCount;
    int MaxSteps;
    int FirstTestSteps;
    double Density;

    SimulationData(std::string fName){
        FILE *fp = fopen(fName.c_str(), "r"); // non-Windows use "r"
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        rapidjson::Document document;
        document.ParseStream(is);

        ParticleCount = document["ParticleCount"].GetInt();
        MaxSteps = document["MaxSteps"].GetInt();
        FirstTestSteps = document["FirstTestStpes"].GetInt();
        Density = document["Density"].GetDouble();
    }
};

#endif //VICSEK_AMPCONSOLE_DATASTRUCTURE_H
