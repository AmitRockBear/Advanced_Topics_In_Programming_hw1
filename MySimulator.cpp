#include "MySimulator.h"
#include "General.h"
#include "Point.h"
#include "Logger.h"
#include "Config.h"
#include "Utils.h"
#include "FileDataExtractor.h"
#include "./Sensors/WallsSensorImpl.h"
#include "./Sensors/DirtSensorImpl.h"
#include "./Sensors/BatteryMeterImpl.h"
#include <iostream>
#include <fstream>

MySimulator::MySimulator(std::size_t stepsTaken, bool missionCompleted, bool missionFailed) 
    : stepsTaken(stepsTaken), missionCompleted(missionCompleted), missionFailed(missionFailed), steps(std::vector<char>()) {
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
        outfile << "NumSteps = " << stepsTaken << std::endl;
        outfile << "DirtLeft = " << house.getTotalDirt() << std::endl;
        if (vacuumCleaner.getBatteryLevel() == 0) {
            outfile << "Status = DEAD" << std::endl;
        } else {
        if (missionCompleted) {
                outfile << "Status = FINISHED" << std::endl;
            } else {
                outfile << "Status = WORKING" << std::endl;
            }
        }

        outfile << "Steps: " << std::endl;
        for(auto &&step : steps) {
            outfile << step << "";
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

    while (!missionCompleted && !missionFailed) {
        vacuumCleaner.getLocation(currentVacuumLocation);

        logger.logInfo("---------------------------------Step: " + std::to_string(stepsTaken) + "---------------------------------");
        logger.logInfo("Dirt level: " + std::to_string(house.getTotalDirt()));
        logger.logInfo("Battery level: " + std::to_string(vacuumCleaner.getBatteryLevel()));
        logger.logInfo("Vacuum cleaner location: " + currentVacuumLocation.toString());
        
        double currentBatteryLevel = vacuumCleaner.getBatteryLevel();
        bool atDockingStation = vacuumCleaner.isAtLocation(dockingLocation);

        // Check if Mission completed
        if (house.getTotalDirt() == 0 && atDockingStation) {
            missionCompleted = true;
            logger.logInfo("Mission completed!");
            continue;
        }

        // Check if mission failed
        if ((currentBatteryLevel == 0 && (!atDockingStation)) || maxSteps - stepsTaken == 0) {
            missionFailed = true;
            logger.logInfo("Mission failed!");
            continue;
        }

        // If vacuumCleaner is at docking station, it loads up until battery is full
        if (currentBatteryLevel < vacuumCleaner.getMaxBatterySteps() && atDockingStation) {
            logger.logInfo("Vacuum cleaner is at docking station, charging...");
            handleDockingStation();
            stepsTaken++;
            steps.push_back(STAY);
            continue;
        }

        // Handle next step
        char nextStep = algorithm.decideNextStep();
        handleNextStep(nextStep);
    }
}

void MySimulator::handleNextStep(char nextStep) {
    Logger& logger = Logger::getInstance();
    Point vacuumCleanerLocation;

    logger.logInfo("Handling next step: " + std::string(1, nextStep));
    vacuumCleaner.getLocation(vacuumCleanerLocation);
    steps.push_back(nextStep);

    // Clean
    if (nextStep == STAY) {
        logger.logInfo("Cleaning...");
        house.decreaseDirtLevel(vacuumCleanerLocation, 1);
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

void MySimulator::setAlgorithm(Algorithm& algo) {
    // MySimulator handles the life time of the sensors so algorithm can use them
    wallsSensor = std::make_unique<WallsSensorImpl>([this](Direction direction) { return this->isWall(direction); });
    dirtSensor = std::make_unique<DirtSensorImpl>([this]() { return this->getDirtLevel(); });
    batteryMeter = std::make_unique<BatteryMeterImpl>([this]() { return this->batteryRemaining(); });
    algo.setMaxSteps(maxSteps);
	algo.setWallsSensor(*wallsSensor);
	algo.setDirtSensor(*dirtSensor);
	algo.setBatteryMeter(*batteryMeter);
    
    algorithm = algo;
}

void MySimulator::readHouseFile(const std::string& fileName) {
    FileDataExtractor inputData = FileDataExtractor();
    inputData.readAndExtract(fileName);
    std::size_t dockingX = inputData.getDockingX();
    std::size_t dockingY = inputData.getDockingY();
    house = House(inputData.getHouseMap(), dockingX, dockingY);
    vacuumCleaner = VacuumCleaner(dockingX, dockingY, inputData.getMaxBattery());
    maxSteps = inputData.getMaxSteps();
    inputFilename = fileName;
    Logger::getInstance().logInfo("MySimulator successfully initialized from file " + fileName);
}
