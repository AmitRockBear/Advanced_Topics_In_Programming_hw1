#include "Controller.h"
#include "General.h"
#include "Point.h"
#include "Logger.h"
#include "Config.h"
#include "Utils.h"
#include <iostream>
#include <fstream>

Controller::Controller(House& house, VacuumCleaner& vacuumCleaner, int maxSteps, const std::string& inputFilename, int stepsTaken, bool missionCompleted, bool missionFailed)
    : house(house), vacuumCleaner(vacuumCleaner),
    algorithm(Algorithm(
            [=, this]() { return this->batteryRemaining(); },
            [=, this](char direction) { return this->isWall(direction); },
            [=, this](char direction) { return this->getDirtLevel(direction); })),
    maxSteps(maxSteps), inputFilename(inputFilename), stepsTaken(stepsTaken), missionCompleted(missionCompleted), missionFailed(missionFailed), steps(std::vector<char>()) {
        Logger::getInstance().logInfo("Controller successfully initialized with maxSteps: " + std::to_string(maxSteps));
    }

void Controller::createOutputFile(const std::string& outputFileName) const {
    Logger& logger = Logger::getInstance();
    std::ofstream outfile;
    bool isEmptyFileName = outputFileName == EMPTY_STRING;
    std::string outputFile;
    if (isEmptyFileName) {
        const std::string filename = getFileBaseName(inputFilename);
        outputFile = "output_" + filename;
    }
    
    logger.logInfo("Creating output file: " + outputFile);
    outfile.open(outputFile, std::ios::out);


    outfile << "Steps preformed: ";
    for(auto &&step : steps) {
        outfile << step << " ";
    }
    outfile << std::endl << "Total steps taken: " << stepsTaken << std::endl;
    outfile << "Dirt remaining in house: " << house.getTotalDirt() << std::endl;
    outfile << (vacuumCleaner.getBatteryLevel() == 0 ? "Vacuum cleaner is dead." : "Vacuum cleaner is alive.") << std::endl;
    outfile << (missionCompleted ? "Mission succeeded!" : "Mission failed.") << std::endl;
    outfile.close();

    logger.logInfo("Output file created successfully");
}


void Controller::run() {
    try {
        Logger::getInstance().logInfo("Starting vacuum cleaner");
        vacuumLoop();
        const std::string& outputFileName = Config::getInstance().get("outputFileName");
        createOutputFile(outputFileName);
    } catch (const std::exception& e) {
        throw std::runtime_error("Unrecoverable error has occured in step: " + std::to_string(stepsTaken) + ". The error is: " + e.what());
    }
}

void Controller::vacuumLoop() {
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

void Controller::handleNextStep(char nextStep) {
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

int Controller::getDirtLevel(char direction) const {
    Point neighbor;
    vacuumCleaner.getLocation(neighbor);
    neighbor.moveToNeighbor(direction);
    return house.getDirtLevel(neighbor);
}

int Controller::isWall(char direction) const {
    Point neighbor;
    vacuumCleaner.getLocation(neighbor);
    neighbor.moveToNeighbor(direction);
    return house.isWall(neighbor);
}

double Controller::batteryRemaining() const {
    return vacuumCleaner.getBatteryLevel();
}

void Controller::handleDockingStation() {
    vacuumCleaner.increaseChargeBy(1);
}