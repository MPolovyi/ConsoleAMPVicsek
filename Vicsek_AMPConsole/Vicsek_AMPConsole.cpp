// Vicsek_AMPConsole.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Helpers/DataHelpers/DataStructures.h"
#include "SimulationRoutines/CSimulationController.h"


void Simulate(std::string fname){
    SimulationData data = SimulationData(fname);

    std::string name = "Results";
    int i = 1;
    while (std::ifstream(name + std::to_string(i) + "_" + fname + ".json")){ i++; }

    std::stringstream s;
    s << name << std::to_string(i) << "_" << fname << ".json";
	std::fstream stream(s.str().c_str(), std::fstream::out);
	cereal::JSONOutputArchive writer(stream);

    CSimulationController::InitAndRun(writer, data);
}

int main(int argc, char** argv)
{
	std::wcout << accelerator(accelerator::default_accelerator).description << std::endl;

	if(argc > 1) {
		Simulate(argv[1]);
	}

	return 0;
}