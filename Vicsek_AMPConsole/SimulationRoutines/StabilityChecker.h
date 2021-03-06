//
// Created by mpolovyi on 15/10/15.
//

#ifndef VICSEK_AMPCONSOLE_STABILITYCHECKER_H
#define VICSEK_AMPCONSOLE_STABILITYCHECKER_H

#include <tuple>

class CIntegrator2D;
struct SimulationData;
struct StabilityCheckData;

class StabilityChecker {
public:
    StabilityChecker(){};
    ~StabilityChecker(){};
    virtual bool Check(CIntegrator2D& integrator, SimulationData& data, StabilityCheckData& stData);
};


#endif //VICSEK_AMPCONSOLE_STABILITYCHECKER_H
