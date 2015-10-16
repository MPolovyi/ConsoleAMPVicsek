//
// Created by mpolovyi on 15/10/15.
//

#include "VelocityDispersionStabilityChecker.h"
#include "Integrator2D.h"
#include <math.h>
#include "../Helpers/DataHelpers/DataStructures.h"

bool VelocityDispersionStabilityChecker::Check(CIntegrator2D &integrator, SimulationData &data,
                                               StabilityCheckData &stData) {
    int testSteps = (int) round(data.FirstTestSteps*0.05);

    integrator.IntegrateWithAveragingFor(testSteps, stData.Noise);
    old_velocity_distribution = integrator.AverVelocityModuleDistribution;
    integrator.IntegrateWithAveragingFor(testSteps, stData.Noise);
    velocity_distribution = integrator.AverVelocityModuleDistribution;

    float aver_old = 0;
    float aver_new = 0;
    float averSq_old = 0;
    float averSq_new = 0;
    for (int i = 0; i < velocity_distribution.size(); i++){
        aver_old += old_velocity_distribution[i];
        aver_new += velocity_distribution[i];

        averSq_old += old_velocity_distribution[i]*old_velocity_distribution[i];
        averSq_new += velocity_distribution[i]*velocity_distribution[i];
    }

    return std::abs(sqrt(aver_old*aver_old - averSq_old) - sqrt(aver_new*aver_new - averSq_new)) <= stData.dispTest;
}
