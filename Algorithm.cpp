#include "Algorithm.h"
#include "General.h"
#include "Logger.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

Algorithm::Algorithm(std::function<double()> batterySensor, std::function<bool(char)> wallSensor,
                     std::function<int(char)> dirtSensor) : batterySensor(batterySensor), wallSensor(wallSensor),
                                                             dirtSensor(dirtSensor), stepsBack(std::stack<char>()), distanceFromDock(Point(0, 0)), isBacktracking(false) {
    std::srand(std::time(nullptr));
}

char Algorithm::oppositeMove(char move) const {
    switch (move) {
        case NORTH: return SOUTH;
        case EAST: return WEST;
        case SOUTH: return NORTH;
        case WEST: return EAST;
        default: return STAY; // In this case, the algorithm will make sure this won't be added to the backtrack path
    }
}

void Algorithm::calcValidMoves(std::vector<char>& moves) {
    Logger& logger = Logger::getInstance();

    logger.logInfo("Calculating valid moves");

    int stepsAmount = stepsBack.size();

    // If there's not enough battery to make a move, the vacuum can only stay in place
    if (batterySensor() < 1) {
        isBacktracking = false;
        moves.push_back(STAY);
        return;
    }

    // If battery left is the same as the amount of steps to the docking station, the vacuum should go back
    if (batterySensor() >= stepsAmount && batterySensor() <= stepsAmount + 1) {
        // In case there are no steps back to perform
        if (stepsBack.empty()) {
            moves.push_back(STAY);
            return;
        }
        isBacktracking = true;
        moves.push_back(stepsBack.top());
        stepsBack.pop();
        return;
    }

    isBacktracking = false;

    // If there's still dirt, the vacuum will stay and clean
    if(dirtSensor(STAY) > 0) {
        moves.push_back(STAY);
        return;
    }

    // Checking other possible directions
    for (auto &&direction : DIRECTIONS) {
        if(!wallSensor(direction)) {
            moves.push_back(direction);
        }
    }
}

char Algorithm::decideNextStep() {
    Logger& logger = Logger::getInstance();

    logger.logInfo("Deciding next step");

    std::vector<char> validMoves = std::vector<char>();
    calcValidMoves(validMoves);

     try {
        if (validMoves.empty()) {
            logger.logInfo("No valid moves, the vacuum cleaner will stay in place");
            return STAY; // Stay in place if no valid moves. C stands for clean
        }

        // Choose the next move randomly
        char nextMove = validMoves[std::rand() % validMoves.size()];
        char oppMove = oppositeMove(nextMove);
        distanceFromDock.move(nextMove);

        // If the vacuum is moving, and it's not backtracking, we want to save its movement as part of the next backtrack path
        if(oppMove != STAY && (!isBacktracking)) {
            logger.logInfo("Vacuum cleaner is moving, saving the opposite move to the backtrack path for future use");
            stepsBack.push(oppMove);
        }

        // If we're back at the docking station, we'd like to empty the backtrack path
        if (distanceFromDock.getX() == 0 && distanceFromDock.getY() == 0) {
            logger.logInfo("Back at the docking station, clearing the backtrack path");
            stepsBack = std::stack<char>();
        }

        logger.logInfo("Next step decided: " + std::string(1, nextMove));
        
        return nextMove;
     } catch (const std::exception& e) {
        logger.logError("Error deciding next step, the vacuum cleaner will stay in place. The error thrown: " + std::string(e.what()));
        return STAY; // Default to staying in place on error
     }
}

// Algorithm::Algorithm() : stepsBack(std::stack<char>()), distanceFromDock(Point(0, 0)), isBacktracking(false) {
//     std::srand(std::time(nullptr));
// }

// void Algorithm::setMaxSteps(int maxSteps) {
//     this->maxSteps = maxSteps;
// }

// void Algorithm::setWallsSensor(const WallsSensor& wallsSensor) {
//     this->wallsSensor = wallsSensor;
// }

// void Algorithm::setDirtSensor(const DirtSensor& dirtSensor) {
//     this->dirtSensor = dirtSensor;
// }

// void Algorithm::setBatteryMeter(const BatteryMeter& batterySensor) {
//     this->batterySensor = batterySensor;
// }
