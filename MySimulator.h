#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include "House.h"
#include "VacuumCleaner.h"
#include "Algorithm.h"
#include "enums.h"
#include "WallsSensorImpl.h"
#include "DirtSensorImpl.h"
#include "BatteryMeterImpl.h"
#include <vector>

class MySimulator {
public:
    explicit MySimulator(std::size_t stepsTaken = 0);
    void run();
    void setAlgorithm(std::unique_ptr<AbstractAlgorithm> algo);
    void readHouseFile(const std::string& fileName);

    MySimulator(const MySimulator&) = delete;
    MySimulator& operator=(const MySimulator&) = delete;
    MySimulator(MySimulator&&) = delete;
    MySimulator& operator=(MySimulator&&) = delete;
private:
    House house;
    VacuumCleaner vacuumCleaner;
    std::size_t maxSteps;
    std::string inputFilename;
    std::size_t stepsTaken;
    bool finished;
    std::vector<Step> steps;
    std::unique_ptr<AbstractAlgorithm> algorithm = nullptr;

    const WallsSensorImpl wallsSensor;
    const DirtSensorImpl dirtSensor;
    const BatteryMeterImpl batteryMeter;

    void vacuumLoop();
    void handleDockingStation();
    int getDirtLevel() const;
    bool isWall(Direction direction) const;
    double batteryRemaining() const;
    void handleNextStep(Step nextStep);
    void createOutputFile(const std::string &outputFileName) const;
};

#endif
