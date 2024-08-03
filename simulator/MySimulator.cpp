#include "MySimulator.h"
#include "../common/General.h"
#include "../common/Point.h"
#include "../common/Logger.h"
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
    {
        Logger::getInstance().logInfo("MySimulator successfully initialized");
    }

void MySimulator::createOutputFile(const std::string& outputFileName) const {
    Logger& logger = Logger::getInstance();
    std::ofstream outfile;
    bool isEmptyFileName = outputFileName == EMPTY_STRING;
    std::string outputFile = outputFileName;
    if (isEmptyFileName) {
        const std::string filename = getFileBaseName(inputFilename);
        outputFile = "output_" + filename;
    }
    
    logger.logInfo("Creating output file: " + outputFile);
    outfile.open(outputFile, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open output file: " + outputFile);
    }
    try {
        Point dockingLocation;
        house.getDockingLocation(dockingLocation);

        outfile << "NumSteps = " << stepsTaken << std::endl;
        outfile << "DirtLeft = " << house.getTotalDirt() << std::endl;
        if (vacuumCleaner.getBatteryLevel() == 0 && !vacuumCleaner.isAtLocation(dockingLocation)) {
            outfile << "Status = DEAD" << std::endl;
        } else {
            if (finished) {
                outfile << "Status = FINISHED" << std::endl;
            } else {
                outfile << "Status = WORKING" << std::endl;
            }
        }

        outfile << "Steps: ";
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

    logger.logInfo("Output file created successfully");
}


void MySimulator::run() {
    if (!algorithm) {
        throw std::runtime_error("Algorithm not set!");
    }
    try {
        Logger::getInstance().logInfo("Starting vacuum cleaner");
        vacuumLoop();
        const std::string& outputFileName = Config::getInstance().get("outputFileName");
        createOutputFile(outputFileName);
    } catch (const std::exception& e) {
        throw std::runtime_error("Unrecoverable error has occured in step: " + std::to_string(stepsTaken) + ". The error is: " + e.what());
    }
}

void MySimulator::vacuumLoop() {
    Logger& logger = Logger::getInstance();
    Point dockingLocation, currentVacuumLocation;
    house.getDockingLocation(dockingLocation);

    while (!finished) {
        vacuumCleaner.getLocation(currentVacuumLocation);

        logger.logInfo("---------------------------------Step: " + std::to_string(stepsTaken) + "---------------------------------");
        logger.logInfo("Dirt level: " + std::to_string(house.getTotalDirt()));
        logger.logInfo("Battery level: " + std::to_string(vacuumCleaner.getBatteryLevel()));
        logger.logInfo("Vacuum cleaner location: " + currentVacuumLocation.toString());
        
        //double currentBatteryLevel = vacuumCleaner.getBatteryLevel();
        bool atDockingStation = vacuumCleaner.isAtLocation(dockingLocation);

        Step nextStep = algorithm->nextStep();
        logger.logInfo("Next step decided: " + toString(nextStep));
        //house.houseVisualization(currentVacuumLocation);

        if(nextStep == Step::Finish) {
            finished = true;
            logger.logInfo("Finished mission!");
            break;
        }

        // If vacuumCleaner is at docking station, and we decided to stay, we'll charge
        if (atDockingStation && nextStep == Step::Stay) {
            logger.logInfo("Vacuum cleaner is at docking station, charging...");
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
    Logger& logger = Logger::getInstance();
    Point vacuumCleanerLocation;

    logger.logInfo("Handling next step: " + toString(nextStep));
    vacuumCleaner.getLocation(vacuumCleanerLocation);
    steps.push_back(nextStep);

    // Clean
    if (nextStep == Step::Stay) {
        logger.logInfo("Cleaning...");
        house.decreaseDirtLevel(vacuumCleanerLocation, 1);
    } else if(nextStep == Step::Finish) {
        logger.logInfo("Finished!");
        return;
    }
    // Otherwise, move
    else {
        vacuumCleaner.move(nextStep);
    }

    vacuumCleaner.decreaseChargeBy(1);
    stepsTaken++;

    vacuumCleaner.getLocation(vacuumCleanerLocation);

    if (Config::getInstance().get("useVisualizer") == "true")
        house.houseVisualization(vacuumCleanerLocation);
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

void MySimulator::setAlgorithm(std::unique_ptr<AbstractAlgorithm> algo) {
    algo->setMaxSteps(maxSteps);
	algo->setWallsSensor(wallsSensor);
	algo->setDirtSensor(dirtSensor);
	algo->setBatteryMeter(batteryMeter);
    algorithm = std::move(algo);
}

void MySimulator::readHouseFile(const std::string& fileName) {
    FileDataExtractor inputData = FileDataExtractor();
    inputData.readAndExtract(fileName);
    ssize_t dockingX = inputData.getDockingX();
    ssize_t dockingY = inputData.getDockingY();
    house.initHouse(inputData.getHouseMap(), dockingX, dockingY);
    vacuumCleaner.initVacuumCleaner(dockingX, dockingY, inputData.getMaxBattery());
    maxSteps = inputData.getMaxSteps();
    inputFilename = fileName;
    Logger::getInstance().logInfo("MySimulator successfully initialized from file " + fileName);
}
