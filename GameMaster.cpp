#include "GameMaster.h"
#include <iostream>
#include <fstream>
#include <sstream>

GameMaster::GameMaster(House& house, VacuumCleaner& vacuumCleaner, int maxSteps, int stepsTaken, bool missionCompleted, bool gameOver) 
    : house(house), vacuumCleaner(vacuumCleaner), algorithm(Algorithm()), maxSteps(maxSteps), stepsTaken(stepsTaken), missionCompleted(missionCompleted), gameOver(gameOver) {}

void GameMaster::run() {
    try {
        gameLoop();
        std::cout << "Cleaning mission completed!" << std::endl;
        std::cout << "Total steps taken: " << stepsTaken << std::endl;
        std::cout << "Remaining battery: " << vacuumCleaner.getBatteryLevel() << std::endl;
        std::cout << "Dirt remaining in house: " << house.getTotalDirt() << std::endl;
        std::cout << (vacuumCleaner.getBatteryLevel() == 0 ? "Vacuum cleaner is dead." : "Vacuum cleaner is alive.") << std::endl;
        std::cout << (missionCompleted ? "Mission succeeded!" : "Mission failed.") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unrecoverable error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown unrecoverable error occurred." << std::endl;
    }
}

void GameMaster::gameLoop() {
    int dockingX = house.getDockingX();
    int dockingY = house.getDockingY();

    while (!missionCompleted && !gameOver) {
        int currentBatteyLevel = vacuumCleaner.getBatteryLevel();
        bool isBackTracking = vacuumCleaner.getIsBackTracking();

        // If vacuumCleaner is at docking station, it loads up until battery is full
        if (currentBatteyLevel < vacuumCleaner.getMaxBatterySteps() && vacuumCleaner.isAtLocation(dockingX, dockingY)) {
            if (!isBackTracking) 
                vacuumCleaner.flipIsBackTrackingStatus();

            handleDockingStation();
            stepsTaken++;
            continue;
        }
        
        int pathSize = vacuumCleaner.getPathSize();
        // Switch to backtrack mode, will continue until vacuumCleaner arrives to docking station
        if (vacuumCleaner.getIsBackTracking() || (maxSteps - stepsTaken) == vacuumCleaner.getPathSize() || 
            currentBatteyLevel == pathSize || currentBatteyLevel == pathSize + 1) {
            handleBacktracking();
            stepsTaken++;
            continue;
        }

        
        int vacuumCleanerX = vacuumCleaner.getX();
        int vacuumCleanerY = vacuumCleaner.getY();
        // Clean
        if (house.getDirtLevel(vacuumCleanerX, vacuumCleanerY) > 0) {
            house.decreaseDirtLevel(vacuumCleanerX, vacuumCleanerY, 1);
            vacuumCleaner.decreaseChargeBy(1);
            stepsTaken++;
            continue;
        }

        /*
            For Michal
            TODO: Implement Algorithm for calculating the next step
            TODO: Update missionCompleted in case the house is cleaned + vacuumCleaner is at docking station
            TODO: Update gameOver in case the vacuumCleaner has 0 battery or (maxSteps - stepsTaken) == 0
            Note: Take a look at handleNextStep(), it might be a good fit. Also in the doc it says that when calculating next step
            it shouldn't be aware of the house so probably we need to move that to the Algorithm class, and maybe we need to do the same
            for the other cases in this while loop 
        */
    }
}

void GameMaster::handleDockingStation() {
    vacuumCleaner.increaseChargeBy(1);
}

void GameMaster::handleBacktracking() {
    char nextStep = vacuumCleaner.backtrack();
    vacuumCleaner.move(nextStep);
}

// void GameMaster::handleNextStep() {
//     std::vector<char> validMoves;
//     char next_step;

//     for (char direction : directions) {
//         Point* nextLocation = vacuumCleaner.calculateStep(direction);
//         if (!house.isWall(*nextLocation)) {
//             validMoves.push_back(direction);
//         }
//         delete nextLocation;
//     }

//     try {
//         next_step = algorithm.decideNextStep(validMoves);
//     } catch (const std::exception& e) {
//         std::cerr << "Recoverable error in deciding next step: " << e.what() << std::endl;
//         next_step = 'S';
//     }

//     try {
//         vacuumCleaner.move(next_step);

//         if (next_step == 'S') {
//             vacuumCleaner.clean();
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Recoverable error during movement/cleaning: " << e.what() << std::endl;
//     }
// }
