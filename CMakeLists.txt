cmake_minimum_required(VERSION 3.29)
project(Simulator CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-std=c++20 -Wall -Wextra -Werror -pedantic)

add_executable(myrobot
        AlgorithmRegistrar.cpp
        BatteryMeterImpl.cpp
        Config.cpp
        Direction.cpp
        DirtSensorImpl.cpp
        FileDataExtractor.cpp
        House.cpp
        Logger.cpp
        myrobot.cpp
        MySimulator.cpp
        Point.cpp
        Utils.cpp
        VacuumCleaner.cpp
        WallsSensorImpl.cpp
)