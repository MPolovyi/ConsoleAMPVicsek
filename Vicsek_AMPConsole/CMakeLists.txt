cmake_minimum_required(VERSION 3.3)
project(Vicsek_AMPConsole)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        Helpers/DataHelpers/JsonHelpers.cpp
        Helpers/DataHelpers/JsonHelpers.h
        Helpers/DataHelpers/DataStructure.h
        Helpers/MathHelpers/MathHelper.cpp
        Helpers/MathHelpers/MathHelper.h
        Rand/amp_rand_collection.h
        Rand/amp_tinymt_rng.h
        Rand/xxamp_tinymt_precalc_dc.h
        Vicsek_AMPConsole.cpp
        Rand/amp_tinymt_rng.cpp
        CommonStructs.h
        DataCollection.h
        Integrator2D.cpp
        Integrator2D.h
        IntegratorCollection.cpp
        IntegratorCollection.h
        Helpers/DataHelpers/Reduction.cpp
        Helpers/DataHelpers/Reduction.h
        stdafx.h
        targetver.h
        Vicsek2DIntegrator.cpp
        Vicsek2DIntegrator.h
        Vicsek2DMath.h
        Vicsek2DMath.cpp
        VicsekKulinsky2DIntegrator.cpp
        VicsekKulinsky2DIntegrator.h
        Viscek2DKulinskIntegrator.cpp
        Viscek2DKulinskIntegrator.h
        CSimulationController.cpp CSimulationController.h)
add_executable(Vicsek_AMPConsole ${SOURCE_FILES})