//
// Created by mpolovyi on 15/10/15.
//

#include "Integrator2D.h"
#include "VelocityDistributionStabilityChecker.h"
#include "../Helpers/DataHelpers/DataStructures.h"
#include <math.h>

bool VelocityDistributionStabilityChecker::Check(CIntegrator2D &integrator, SimulationData& data, StabilityCheckData& stData) {
    integrator.IntegrateWithAveragingFor(stData.testStepsCount, stData.Noise);
    old_velocity_distribution = integrator.AverVelocityModuleDistribution;
    integrator.IntegrateWithAveragingFor(stData.testStepsCount, stData.Noise);
    velocity_distribution = integrator.AverVelocityModuleDistribution;

    bool ret = true;
    for (int i = 0; i < velocity_distribution.size(); i++){
        ret = ret && (std::abs(velocity_distribution[i]-old_velocity_distribution[i]) < stData.dispTest);
    }
    return ret;
}
