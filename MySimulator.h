#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include "House.h"
#include "VacuumCleaner.h"
#include "Algorithm.h"
#include "Direction.h"
#include <vector>
#include <memory>

class MySimulator {
public:
    MySimulator(bool missionCompleted, bool missionFailed, Algorithm &algorithm,
                VacuumCleaner vacuumCleaner, House house);
    void run();
    void setAlgorithm(Algorithm& algorithm);
    void readHouseFile(const std::string& fileName);
private:
    House house;
    VacuumCleaner vacuumCleaner;
    //std::unique_ptr<Algorithm> algorithm;
    Algorithm* algorithm;
    std::size_t maxSteps{};
    std::string inputFilename;
    std::size_t stepsTaken = 0;
    bool missionCompleted;
    bool missionFailed;
    std::vector<Step> steps;
    std::unique_ptr<WallsSensor> wallsSensor;
    std::unique_ptr<DirtSensor> dirtSensor;
    std::unique_ptr<BatteryMeter> batteryMeter;

    void vacuumLoop();
    void handleDockingStation();
    int getDirtLevel() const;
    bool isWall(Direction direction) const;
    double batteryRemaining() const;
    void handleNextStep(Step nextStep);
    void createOutputFile(const std::string &outputFileName) const;
};

#endif
