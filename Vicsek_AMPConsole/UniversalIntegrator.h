#pragma once
#include "CommonStructs.h"
#include "Rand\amp_tinymt_rng.h"
#include <string>
// Class for managing particle=particle interractions.
// TsData - struct of type TaskData, for distinguishing betveen 2D and 3D comp
// BorderInts - struct for particle-border interraction, PPint - particle=particle
template <class TsData, class BorderInt, class PPint>
class CUniversalIntegrator
{
public:
	CUniversalIntegrator(SimulationStats stats) {};
	~CUniversalIntegrator() {};

	std::string SerializeFull();
	std::string SerializeParticlesData();
	std::string SerializeSimStats();
	std::string SerializeSimParams();
private:
	void InterractionTimeStep();
	
	tinymt_collection<1> m_Rnd = tinymt_collection<1>(extent<1>(1), std::rand());

};

