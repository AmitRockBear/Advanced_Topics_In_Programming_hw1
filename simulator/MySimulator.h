#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include "House.h"
#include "VacuumCleaner.h"
#include "FileDataExtractor.h"
#include "../algorithms/Algorithm.h"
#include "../common/enums.h"
#include "../common/WallsSensorImpl.h"
#include "../common/DirtSensorImpl.h"
#include "../common/BatteryMeterImpl.h"
#include <vector>

class MySimulator {
public:
    explicit MySimulator(std::size_t stepsTaken = 0);
    void run();
    void setAlgorithm(AbstractAlgorithm& algo, const std::string& algoName);
    void initSimulator(FileDataExtractor& inputData, const std::string& fileName);
    void setIsSummaryOnly(bool isSummary);
    std::size_t getTotalDirt() const;
    std::size_t getMaxSteps() const;
    std::size_t getScore() const;
    std::string getHouseName() const;
    std::string getAlgorithmName() const;

    MySimulator(const MySimulator&) = delete;
    MySimulator& operator=(const MySimulator&) = delete;
    MySimulator(MySimulator&&) = delete;
    MySimulator& operator=(MySimulator&&) = delete;
private:
    std::size_t score;
    House house;
    VacuumCleaner vacuumCleaner;
    std::size_t maxSteps;
    std::string houseName;
    std::string algorithmName;
    std::size_t stepsTaken;
    bool finished;
    std::vector<Step> steps;
    AbstractAlgorithm* algorithm = nullptr;
    bool isSummaryOnly = false;

    const WallsSensorImpl wallsSensor;
    const DirtSensorImpl dirtSensor;
    const BatteryMeterImpl batteryMeter;

    void vacuumLoop();
    void handleDockingStation();
    int getDirtLevel() const;
    bool isWall(Direction direction) const;
    double batteryRemaining() const;
    void handleNextStep(Step nextStep);
    void createOutputFile();
    void initHouse(FileDataExtractor& inputData, const std::string& fileName);
    void calculateScore();
    bool isVacuumDead() const;
    bool isVacuumAtDocking() const;
};

#endif