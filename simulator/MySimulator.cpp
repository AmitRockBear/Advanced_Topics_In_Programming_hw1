#include "MySimulator.h"
#include "../common/General.h"
#include "../common/Point.h"
#include "../common/Config.h"
#include "../common/Utils.h"
#include "FileDataExtractor.h"
#include "../common/Direction.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <utility>

MySimulator::MySimulator(std::size_t stepsTaken) 
    : stepsTaken(stepsTaken), finished(false), steps(std::vector<Step>()), 
    wallsSensor(WallsSensorImpl([this](Direction direction) { return this->isWall(direction); })),
    dirtSensor(DirtSensorImpl([this]() { return this->getDirtLevel(); })),
    batteryMeter(BatteryMeterImpl([this]() { return this->batteryRemaining(); }))
    {}

bool MySimulator::isVacuumAtDocking() const {
    Point dockingLocation;
    house.getDockingLocation(dockingLocation);
    return vacuumCleaner.isAtLocation(dockingLocation);
}

bool MySimulator::isVacuumDead() const {
    return vacuumCleaner.getBatteryLevel() == 0 && !isVacuumAtDocking();
}

void MySimulator::calculateScore() {
    bool isAtDocking = isVacuumAtDocking();
    bool isDead = isVacuumDead();
    std::size_t dirtLeft = getTotalDirt();
    score = stepsTaken + dirtLeft * 300 + (isAtDocking ? 0 : 1000);
    if (isDead) score = maxSteps + dirtLeft * 300 + 2000;
    else if (finished && !isAtDocking) score = maxSteps + dirtLeft * 300 + 3000;
}

void MySimulator::createOutputFile(const std::string& outputFileName) {
    std::ofstream outfile;
    bool isEmptyFileName = outputFileName == EMPTY_STRING;
    std::string outputFile = outputFileName;
    if (isEmptyFileName) {
        outputFile = houseName + "-" + algorithmName + ".txt";
    }

    outfile.open(outputFile, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open output file: " + outputFile);
    }
    try {
        bool isAtDocking = isVacuumAtDocking();
        bool isDead = isVacuumDead();
        std::size_t dirtLeft = getTotalDirt();

        outfile << "NumSteps = " << stepsTaken << std::endl;
        outfile << "DirtLeft = " << dirtLeft << std::endl;
        if (isDead) {
            outfile << "Status = DEAD" << std::endl;
        } else {
            if (finished) {
                outfile << "Status = FINISHED" << std::endl;
            } else {
                outfile << "Status = WORKING" << std::endl;
            }
        }
        if (isAtDocking) {
            outfile << "InDock = TRUE" << std::endl;
        } else {
            outfile << "InDock = FALSE" << std::endl;
        }
        outfile << "Score = " << score << std::endl;
        outfile << "Steps: " << std::endl;
        for(auto &&step : steps) {
            outfile << toString(step);
        }
    } catch(const std::exception& e) {
        outfile.close();
        throw std::runtime_error("Error during writing to output file: " + outputFile + ". The error is: " + e.what());
    } catch(...) {
        outfile.close();
        throw std::runtime_error("Unknown error during writing to output file: " + outputFile);
    }
    outfile.close();
}


void MySimulator::run() {
    if (!algorithm) {
        throw std::runtime_error("Algorithm not set!");
    }
    try {
        vacuumLoop();
        calculateScore();
        if (!isSummaryOnly) {
            const std::string& outputFileName = Config::getInstance().get("outputFileName");
            createOutputFile(outputFileName);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Unrecoverable error has occured in step: " + std::to_string(stepsTaken) + ". The error is: " + e.what());
    }
}

void MySimulator::vacuumLoop() {
    Point dockingLocation, currentVacuumLocation;
    house.getDockingLocation(dockingLocation);

    while (!finished) {
        vacuumCleaner.getLocation(currentVacuumLocation);
        bool atDockingStation = vacuumCleaner.isAtLocation(dockingLocation);

        Step nextStep = algorithm->nextStep();

        if(nextStep == Step::Finish) {
            finished = true;
            break;
        }

        // If vacuumCleaner is at docking station, and we decided to stay, we'll charge
        if (atDockingStation && nextStep == Step::Stay) {
            handleDockingStation();
            stepsTaken++;
            steps.push_back(Step::Stay);
        }
        else {
            // Handle next step
            handleNextStep(nextStep);
        }
    }
}

void MySimulator::handleNextStep(Step nextStep) {
    Point vacuumCleanerLocation;

    vacuumCleaner.getLocation(vacuumCleanerLocation);
    steps.push_back(nextStep);

    // Clean
    if (nextStep == Step::Stay) {
        house.decreaseDirtLevel(vacuumCleanerLocation, 1);
    } else if(nextStep == Step::Finish) {
        return;
    }
    // Otherwise, move
    else {
        vacuumCleaner.move(nextStep);
    }

    vacuumCleaner.decreaseChargeBy(1);
    stepsTaken++;
}

int MySimulator::getDirtLevel() const {
    Point curr;
    vacuumCleaner.getLocation(curr);
    return house.getDirtLevel(curr);
}

bool MySimulator::isWall(Direction direction) const {
    Point neighbor;
    vacuumCleaner.getLocation(neighbor);
    neighbor.moveToNeighbor(direction);
    return house.isWall(neighbor);
}

double MySimulator::batteryRemaining() const {
    return vacuumCleaner.getBatteryLevel();
}

void MySimulator::handleDockingStation() {
    vacuumCleaner.increaseChargeBy(1);
}

void MySimulator::setAlgorithm(AbstractAlgorithm& algo, const std::string& algoName) {
    algo.setMaxSteps(maxSteps);
    algo.setWallsSensor(wallsSensor);
    algo.setDirtSensor(dirtSensor);
    algo.setBatteryMeter(batteryMeter);
    algorithm = &algo;
    algorithmName = getFileBaseName(algoName);
}

std::size_t MySimulator::getTotalDirt() const {
    return house.getTotalDirt();
}

std::size_t MySimulator::getMaxSteps() const {
    return maxSteps;
}

std::size_t MySimulator::getScore() const {
    return score;
}

void MySimulator::initHouse(FileDataExtractor& inputData, const std::string& fileName) {
    house.initHouse(inputData.getHouseMap(), inputData.getDockingX(), inputData.getDockingY());
    houseName = getFileBaseName(fileName);
}

void MySimulator::initSimulator(FileDataExtractor& inputData, const std::string& fileName) {
    initHouse(inputData, fileName);
    vacuumCleaner.initVacuumCleaner(inputData.getDockingX(), inputData.getDockingY(), inputData.getMaxBattery());
    maxSteps = inputData.getMaxSteps();
}

void MySimulator::setIsSummaryOnly(bool isSummary) {
    isSummaryOnly = isSummary;
}

std::string MySimulator::getHouseName() const {
    return houseName;
}

std::string MySimulator::getAlgorithmName() const {
    return algorithmName;
}