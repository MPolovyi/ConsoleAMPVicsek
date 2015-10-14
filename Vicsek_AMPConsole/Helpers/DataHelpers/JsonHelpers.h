//
// Created by mpolovyi on 14/10/15.
//

#ifndef VICSEK_AMPCONSOLE_JSONHELPERS_H
#define VICSEK_AMPCONSOLE_JSONHELPERS_H

#include "../../rapidjson/stringbuffer.h"

#include "../../rapidjson/filewritestream.h"
#include "../../rapidjson/prettywriter.h"

class CSimulationController;

void WriteData(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, CSimulationController& simContr);

void WriteSimParams(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, CSimulationController&simContr);

void EndDataFlow(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer);

#endif //VICSEK_AMPCONSOLE_JSONHELPERS_H
