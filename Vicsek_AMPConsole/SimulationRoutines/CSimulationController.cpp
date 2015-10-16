//
// Created by mpolovyi on 14/10/15.
//

#include <math.h>
#include "../Helpers/MathHelpers/MathHelper.h"
#include "CSimulationController.h"
#include "Vicsek2DIntegrator.h"
#include "VicsekKulinsky2DIntegrator.h"
#include "StabilityChecker.h"
#include "VelocityDispersionStabilityChecker.h"
#include "VelocityDistributionStabilityChecker.h"

#include <amp_graphics.h>

using concurrency::graphics::float_2;

void CSimulationController::InitAndRun(rapidjson::PrettyWriter<rapidjson::FileWriteStream> &writer, SimulationData simData) {
    CSimulationController* sim = new CSimulationController();
    sim->m_SimData = &simData;
    sim->m_Data = new TaskData2D(simData.ParticleCount, accelerator(accelerator::default_accelerator).default_view, accelerator(accelerator::default_accelerator));

    switch (simData.BCond) {
        case BorderConditions::Transitional:
            sim->m_Integrator = new CVicsek2DIntegrator(*sim->m_Data, float_2(simData.SystemSizeX, simData.SystemSizeY));
            break;
        case BorderConditions::Kuette:
            sim->m_Integrator = (CVicsek2DIntegrator*) new CVicsekKulinsky2DIntegrator(*sim->m_Data, float_2(simData.SystemSizeX, simData.SystemSizeY));
            break;
        default:
            sim->m_Integrator = new CVicsek2DIntegrator(*sim->m_Data, float_2(simData.SystemSizeX, simData.SystemSizeY));
            break;
    }
    StartDataFlow(writer);

	sim->m_stCheckerCount = 2;
	sim->m_StabilityChecker = new StabilityChecker*[2];

    sim->m_StabilityChecker[0] = new VelocityDispersionStabilityChecker();
	sim->m_StabilityChecker[1] = new StabilityChecker();

    int firstSteps = simData.FirstTestSteps;
    StabilityCheckData stCheckData;
    stCheckData.dispTest = 0.000001;
    stCheckData.testStepsCount = (int) round(simData.FirstTestSteps*0.05);

    for (auto noise = simData.MaxNoise; noise > simData.MinNoise; noise+=simData.StepNoise){
		bool isStable = false;
		std::cout << "Simulating noise " << noise << std::endl;
		while (!isStable)
		{
			sim->Noise = noise;
			sim->m_Integrator->IntegrateFor(firstSteps, sim->Noise);
			stCheckData.Noise = sim->Noise;
			for (int i = 0; i < sim->m_stCheckerCount; i++) {
				isStable = isStable || sim->m_StabilityChecker[i]->Check(*sim->m_Integrator, simData, stCheckData);
			}
			WriteData(writer, *sim);
			std::cout << "Sim steps = " << (*sim->m_Integrator).Steps << std::endl;
		}
    }
    EndDataFlow(writer);

    delete sim;
}

std::vector<float> &CSimulationController::GetVelocityDistribution() {
    auto tmp = m_Integrator->GetAverVelocityDistributionY(10);

    std::vector<float> ret;
    ret.resize(tmp.size());

    for(auto vel : tmp){
        ret.push_back(MathHelpers::Length(vel));
    }

    return ret;
}

std::vector<float> &CSimulationController::GetDensityDistribution() {
	return m_Integrator->GetAverDensityDistributionY(10);
}