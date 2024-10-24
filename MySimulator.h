#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include "House.h"
#include "VacuumCleaner.h"
#include "Algorithm.h"
#include "skeleton.h"
#include <vector>

class MySimulator {
public:
    MySimulator(std::size_t stepsTaken = 0, bool missionCompleted = false, bool missionFailed = false);
    void run();
    void setAlgorithm(Algorithm& algorithm);
    void readHouseFile(const std::string& fileName);
private:
    House house;
    VacuumCleaner vacuumCleaner;
    Algorithm& algorithm;
    std::size_t maxSteps;
    std::string& inputFilename;
    std::size_t stepsTaken;
    bool missionCompleted;
    bool missionFailed;
    std::vector<char> steps;
    std::unique_ptr<WallsSensor> wallsSensor;
    std::unique_ptr<DirtSensor> dirtSensor;
    std::unique_ptr<BatteryMeter> batteryMeter;

    void vacuumLoop();
    void handleDockingStation();
    int getDirtLevel() const;
    bool isWall(Direction direction) const;
    double batteryRemaining() const;
    void handleNextStep(char nextStep);
    void createOutputFile(const std::string &outputFileName) const;
};

#endif
