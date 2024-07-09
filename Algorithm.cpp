#include "Algorithm.h"
#include "General.h"
#include "Logger.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

Algorithm::Algorithm(BatteryMeter &batterySensor, DirtSensor &dirtSensor, WallsSensor &wallsSensor)
        : stepsBack(std::stack<Step>()), distanceFromDock(Point(0, 0)), isBacktracking(false),
          wallsSensor(wallsSensor), dirtSensor(dirtSensor), batterySensor(batterySensor) {
    std::srand(std::time(nullptr));
}

Step Algorithm::oppositeMove(Step move) const {
    switch (move) {
        case Step::North: return Step::South;
        case Step::East: return Step::West;
        case Step::South: return Step::North;
        case Step::West: return Step::East;
        default: return Step::Stay; // In this case, the algorithm will make sure this won't be added to the backtrack path
    }
}

void Algorithm::calcValidMoves(std::vector<Step>& moves) {
    Logger& logger = Logger::getInstance();

    logger.logInfo("Calculating valid moves");

    int stepsAmount = stepsBack.size();

    // If there's not enough battery to make a move, the vacuum can only stay in place
    if (batterySensor.getBatteryState() < 1) {
        isBacktracking = false;
        moves.push_back(Step::Stay);
        return;
    }

    // If battery left is the same as the amount of steps to the docking station, the vacuum should go back
    if (batterySensor.getBatteryState() >= (size_t) stepsAmount && batterySensor.getBatteryState() <= (size_t) stepsAmount + 1) {
        // In case there are no steps back to perform
        if (stepsBack.empty()) {
            moves.push_back(Step::Stay);
            return;
        }
        isBacktracking = true;
        moves.push_back(stepsBack.top());
        stepsBack.pop();
        return;
    }

    isBacktracking = false;

    // If there's still dirt, the vacuum will stay and clean
    if(dirtSensor.dirtLevel() > 0) {
        moves.push_back(Step::Stay);
        return;
    }

    // Checking other possible directions
    for (auto &&direction : DIRECTIONS) {
        if(!wallsSensor.isWall(direction)) {
            moves.push_back(DirectionToStep(direction));
        }
    }
}

Step Algorithm::decideNextStep() {
    Logger& logger = Logger::getInstance();

    logger.logInfo("Deciding next step");

    std::vector<Step> validMoves = std::vector<Step>();
    calcValidMoves(validMoves);

     try {
        if (validMoves.empty()) {
            logger.logInfo("No valid moves, the vacuum cleaner will stay in place");
            return Step::Stay; // Stay in place if no valid moves. C stands for clean
        }

        // Choose the next move randomly
        Step nextMove = validMoves[std::rand() % validMoves.size()];
         Step oppMove = oppositeMove(nextMove);
        distanceFromDock.move(nextMove);

        // If the vacuum is moving, and it's not backtracking, we want to save its movement as part of the next backtrack path
        if(oppMove != Step::Stay && (!isBacktracking)) {
            logger.logInfo("Vacuum cleaner is moving, saving the opposite move to the backtrack path for future use");
            stepsBack.push(oppMove);
        }

        // If we're back at the docking station, we'd like to empty the backtrack path
        if (distanceFromDock.getX() == 0 && distanceFromDock.getY() == 0) {
            logger.logInfo("Back at the docking station, clearing the backtrack path");
            stepsBack = std::stack<Step>();
        }

        //logger.logInfo("Next step decided: " + std::string(1, nextMove));
        
        return nextMove;
     } catch (const std::exception& e) {
        logger.logError("Error deciding next step, the vacuum cleaner will stay in place. The error thrown: " + std::string(e.what()));
        return Step::Stay; // Default to staying in place on error
     }
}

 void Algorithm::setMaxSteps(std::size_t maxSteps) {
     this->maxSteps = maxSteps;
 }

 void Algorithm::setWallsSensor(const WallsSensor& wallsSensor) {
     this->wallsSensor = wallsSensor;
 }

 void Algorithm::setDirtSensor(const DirtSensor& dirtSensor) {
     this->dirtSensor = dirtSensor;
 }

 void Algorithm::setBatteryMeter(const BatteryMeter& batterySensor) {
     this->batterySensor = batterySensor;
 }

