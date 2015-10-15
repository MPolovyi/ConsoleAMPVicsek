//
// Created by mpolovyi on 15/10/15.
//

#ifndef VICSEK_AMPCONSOLE_VELOCITYDISTRIBUTIONSTABILITYCHECKER_H
#define VICSEK_AMPCONSOLE_VELOCITYDISTRIBUTIONSTABILITYCHECKER_H

#include <vector>
#include "StabilityChecker.h"

class VelocityDistributionStabilityChecker :
        public StabilityChecker {
public:
    virtual bool Check(CIntegrator2D& integrator, SimulationData& data, StabilityCheckData& stData);

protected:
    std::vector<float> old_velocity_distribution;
    std::vector<float> velocity_distribution;
};


#endif //VICSEK_AMPCONSOLE_VELOCITYDISTRIBUTIONSTABILITYCHECKER_H
