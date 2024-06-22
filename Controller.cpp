#include "Controller.h"
#include <iostream>
#include <fstream>
#include "General.h"
#include "Point.h"

Controller::Controller(House& house, VacuumCleaner& vacuumCleaner, int maxSteps, int stepsTaken, bool missionCompleted, bool missionFailed)
    : house(house), vacuumCleaner(vacuumCleaner),
    algorithm(Algorithm(
            [=]() { return this->batteryRemaining(); },
            [=](char direction) { return this->isWall(direction); },
            [=](char direction) { return this->getDirtLevel(direction); })),
    maxSteps(maxSteps), stepsTaken(stepsTaken), missionCompleted(missionCompleted), missionFailed(missionFailed) {
    steps = std::vector<char>();
}

void Controller::run() {
    try {
        vacuumLoop();
        std::ofstream outfile (OUTPUT_FILE_NAME);
        outfile << "Steps preformed: ";
        for(auto &&step : steps) {
            outfile << step << " ";
        }
        outfile << std::endl << "Total steps taken: " << stepsTaken << std::endl;
        outfile << "Dirt remaining in house: " << house.getTotalDirt() << std::endl;
        outfile << (vacuumCleaner.getBatteryLevel() == 0 ? "Vacuum cleaner is dead." : "Vacuum cleaner is alive.") << std::endl;
        outfile << (missionCompleted ? "Mission succeeded!" : "Mission failed.") << std::endl;
        outfile.close();
    } catch (const std::exception& e) {
        std::cerr << "Unrecoverable error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown unrecoverable error occurred." << std::endl;
    }
}



void Controller::vacuumLoop() {
    int dockingX = house.getDockingX();
    int dockingY = house.getDockingY();

    while (!missionCompleted && !missionFailed) {
        double currentBatteryLevel = vacuumCleaner.getBatteryLevel();
        bool atDockingStation = vacuumCleaner.isAtLocation(dockingX, dockingY);

        // Check if Mission completed
        if (house.getTotalDirt() == 0 && atDockingStation) {
            missionCompleted = true;
            continue;
        }

        // Check if Mission failed
        if ((currentBatteryLevel == 0 && (!atDockingStation)) || maxSteps - stepsTaken == 0) {
            missionFailed = true;
            continue;
        }

        // If vacuumCleaner is at docking station, it loads up until battery is full
        if (currentBatteryLevel < vacuumCleaner.getMaxBatterySteps() &&atDockingStation) {
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
    int vacuumCleanerX = vacuumCleaner.getX();
    int vacuumCleanerY = vacuumCleaner.getY();

    steps.push_back(nextStep);

    // Clean
    if (nextStep == STAY) {
        house.decreaseDirtLevel(vacuumCleanerX, vacuumCleanerY, 1);
    }
    // Otherwise, move
    else {
        vacuumCleaner.move(nextStep);
    }

    vacuumCleaner.decreaseChargeBy(1);
    stepsTaken++;
}

int Controller::getDirtLevel(char direction) const {
    Point neighbor = vacuumCleaner.getLocation();
    neighbor.moveToNeighbor(direction);
    return house.getDirtLevel(neighbor);
}

int Controller::isWall(char direction) const {
    Point neighbor = vacuumCleaner.getLocation();
    neighbor.moveToNeighbor(direction);
    return house.isWall(neighbor);
}

double Controller::batteryRemaining() const {
    return vacuumCleaner.getBatteryLevel();
}

void Controller::handleDockingStation() {
    vacuumCleaner.increaseChargeBy(1);
}
