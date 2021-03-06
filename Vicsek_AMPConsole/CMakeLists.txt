cmake_minimum_required(VERSION 3.3)
project(Vicsek_AMPConsole)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} /LTCG:incremental")

set(SOURCE_FILES
        Helpers/DataHelpers/JsonHelpers.cpp
        Helpers/DataHelpers/JsonHelpers.h
        Helpers/DataHelpers/DataStructures.h
        Helpers/MathHelpers/Reduction.h
        Helpers/MathHelpers/MathHelper.cpp
        Helpers/MathHelpers/MathHelper.h
        Helpers/MathHelpers/Vicsek2DMath.h
        Helpers/MathHelpers/Vicsek2DMath.cpp
        Rand/amp_rand_collection.h
        Rand/amp_tinymt_rng.h
        Rand/xxamp_tinymt_precalc_dc.h
        Rand/amp_tinymt_rng.cpp
        SimulationRoutines/CommonStructs.h
        SimulationRoutines/Integrator2D.h
        SimulationRoutines/Integrator2D.cpp
        SimulationRoutines/CSimulationController.h
        SimulationRoutines/CSimulationController.cpp
        SimulationRoutines/StabilityChecker.h
        SimulationRoutines/StabilityChecker.cpp
        SimulationRoutines/VelocityDistributionStabilityChecker.h
        SimulationRoutines/VelocityDistributionStabilityChecker.cpp
        SimulationRoutines/VelocityDispersionStabilityChecker.h
        SimulationRoutines/VelocityDispersionStabilityChecker.cpp
        SimulationRoutines/Vicsek2DIntegrator.h
        SimulationRoutines/Vicsek2DIntegrator.cpp
	SimulationRoutines/VicsekChepizhko2DIntegrator.h
	SimulationRoutines/VicsekChepizhko2DIntegrator.cpp
        SimulationRoutines/VicsekKulinsky2DIntegrator.h
        SimulationRoutines/VicsekKulinsky2DIntegrator.cpp
        Vicsek_AMPConsole.cpp
)
add_executable(Vicsek_AMPConsole ${SOURCE_FILES})