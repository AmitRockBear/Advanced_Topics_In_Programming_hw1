#include "Algorithm.h"
#include "General.h"
#include "Logger.h"
#include "BatteryMeterImpl.h"
#include "DirtSensorImpl.h"
#include "WallsSensorImpl.h"
#include "Point.h"
#include <ctime>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <queue>
#include <unordered_set>

Algorithm::Algorithm(): dockingStation(Point(0,0)), distanceFromDock(dockingStation), maxSteps(0), maxBattery(0), stepsLeft(0), isBacktracking(false), isGoingForward(false), firstMove(true) {
    knownPoints[dockingStation] = PointState::wip;
}

//void Algorithm::calcValidMoves(std::vector<Step>& moves) {
//    Logger& logger = Logger::getInstance();
//
//    logger.logInfo("Calculating valid moves");
//
//    size_t stepsAmount = stepsBack.size();
//
//    // If there's not enough battery to make a move, the vacuum can only stay in place
//    if (batterySensor->getBatteryState() < 1) {
//        isBacktracking = false;
//        moves.push_back(Step::Stay);
//        return;
//    }
//
//    // If the battery level is insufficient relative to the distance to the docking station, the vacuum should go back
//    if (batterySensor->getBatteryState() >= stepsAmount && batterySensor->getBatteryState() <= stepsAmount + 1) {
//        // In case there are no steps back to perform
//        if (stepsBack.empty()) {
//            moves.push_back(Step::Stay);
//            return;
//        }
//        isBacktracking = true;
//        moves.push_back(stepsBack.top());
//        stepsBack.pop();
//        return;
//    }
//
//    isBacktracking = false;
//
//    // If there's still dirt, the vacuum will stay and clean
//    if(dirtSensor->dirtLevel() > 0) {
//        moves.push_back(Step::Stay);
//        return;
//    }
//
//    // Checking other possible directions
//    for (auto &&direction : DIRECTIONS) {
//        if(!wallsSensor->isWall(direction)) {
//            moves.push_back(DirectionToStep(direction));
//        }
//    }
//}

//Step Algorithm::nextStep() {
//    if (firstMove) {
//        maxBattery = batterySensor->getBatteryState();
//        firstMove = false;
//    }
//    Logger& logger = Logger::getInstance();
//    logger.logInfo("Deciding next step");
//    try {
//        bool atDockingStation = (distanceFromDock.getX() == 0 && distanceFromDock.getY() == 0);
//
//        std::vector<Step> validMoves = std::vector<Step>();
//        calcValidMoves(validMoves);
//
//        if (validMoves.empty()) {
//            // If we're back at the docking station and there are no valid moves, return Finish
//            if(atDockingStation) {
//                return Step::Finish;
//            }
//            else {
//                logger.logInfo("No valid moves, the vacuum cleaner will stay in place");
//                return Step::Stay; // Remain in place if there are no valid moves
//            }
//        }
//
//        Step nextMove = validMoves[0]; // If there are multiple smart steps available, we'll choose the first
//        Step oppMove = oppositeStep(nextMove);
//        distanceFromDock.move(nextMove);
//        pointsColors[distanceFromDock] = Color::grey; // Update current node
//
//        // If the vacuum is moving, and it's not backtracking, we want to save its movement as part of the next backtrack path
//        if(oppMove != Step::Stay && (!isBacktracking)) {
//            logger.logInfo("Vacuum cleaner is moving, saving the opposite move to the backtrack path for future use");
//            stepsBack.push(oppMove);
//        }
//
//        // If we're back at the docking station, we'd like to empty the backtrack path
//        if (atDockingStation) {
//            logger.logInfo("Back at the docking station, clearing the backtrack path");
//            stepsBack = std::stack<Step>();
//        }
//
//
//        //logger.logInfo("Next step decided: " + std::string(1, nextMove));
//
//        return nextMove;
//    } catch (const std::exception& e) {
//        logger.logError("Error deciding next step, the vacuum cleaner will stay in place. The error thrown: " + std::string(e.what()));
//        return Step::Stay; // Default to staying in place on error
//    }
//}

// Fills the vector newMoves with all possible moves to neighbor points that we haven't visited yet
void Algorithm::calcNewMoves(std::vector<Step>& newMoves) {
    bool isWall, notVisited;
    Point neighbor;
    for (auto &&direction : DIRECTIONS) {
        neighbor = distanceFromDock.getNeighbor(direction);
        isWall = wallsSensor->isWall(direction);
        notVisited = (!visitedPoints.contains(neighbor));
        if((!isWall) && notVisited) {
            knownPoints.insert(neighbor);
            newMoves.push_back(DirectionToStep(direction));
        }
    }
}

// Take a step back as part of the backtracking to the docking station
Step Algorithm::stepBack() {
    Step nextStep = stepsBackToDocking.top();
    stepsBackToDocking.pop();
    stepsFromDocking.push(oppositeStep(nextStep));
    distanceFromDock.move(nextStep);
    return nextStep;
}

// Take a forward step back as part of the path back from the docking station
Step Algorithm::stepForward() {
    Step nextStep = stepsFromDocking.top();
    stepsFromDocking.pop();
    if(stepsBackToDocking.empty()) {
        isGoingForward = false;
    }
    distanceFromDock.move(nextStep);
    return nextStep;
}

// Go back to the parent (the point from which we came from)
Step Algorithm::backToParent() {
    Step nextStep = stepsBack.top();
    stepsBack.pop();
    distanceFromDock.move(nextStep);
    return nextStep;
}

// Returns stack of steps, that combine the shortest path from source to target (based on points we currently know!)
std::stack<Step> Algorithm::findShortestPath(Point source, Point target) {
    std::queue<Point> q = std::queue<Point>();
    std::unordered_set<Point> visited = std::unordered_set<Point>();
    std::unordered_map<Point, Point> prev = std::unordered_map<Point, Point>();
    Logger& logger = Logger::getInstance();
    Point current;
    q.push(source);
    visited.insert(source);

    while(!q.empty()) {
        current = q.front();
        q.pop();
        if (current == target) {
            break;
        }
        std::vector<Point> current_neighbors = std::vector<Point>();
        for (auto &&direction : DIRECTIONS) {
            Point neighbor = current.getNeighbor(direction);
            if (knownPoints.contains(neighbor))
                current_neighbors.push_back(neighbor);
        }
        for (auto &&neighbor : current_neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
                prev[neighbor] = current;
            }
        }
    }
    // No path found
    if (!(current == target)) {
        logger.logError("Error finding shortest path");
        return {};
    }
    std::stack<Step> steps;
    while (true) {
        if (current == source)
            break;
        steps.push(oppositeStep(current.getStepToGetToNeighborPoint(prev[current])));
        current = prev[current];
    }
    return steps;
}

Step Algorithm::nextStep() {
    if (firstMove) {
        maxBattery = batterySensor->getBatteryState();
        firstMove = false;
    }

    Logger& logger = Logger::getInstance();
    logger.logInfo("Deciding next step");

    // Find the next step
    try {
        bool atDockingStation = (distanceFromDock == dockingStation);

        // If we're back at the docking station, we'd like to empty the backtrack path
        if (atDockingStation) {
            logger.logInfo("Back at the docking station, clearing the backtrack path");
            stepsBackToDocking = std::stack<Step>();
            if (isBacktracking) {
                isGoingForward = true;
            }
            isBacktracking = false;
        }
        else {
            // Update steps back based on current shortest path - We check it everytime because the path is based on the points we've been to so far
            stepsBackToDocking = findShortestPath(distanceFromDock, dockingStation);

        }

        std::vector<Step> newMoves = std::vector<Step>(); // WhiteMoves = Moves to neighboring points we haven't visited yet
        calcNewMoves(newMoves);

        size_t stepsToDockingStation = stepsBackToDocking.size();
        bool batteryInsufficient = (batterySensor->getBatteryState() >= stepsToDockingStation && batterySensor->getBatteryState() <= stepsToDockingStation + 1);
        bool stepsInsufficient = (stepsToDockingStation >= stepsLeft);

        logger.logInfo("Steps left: " +   std::to_string(stepsLeft));
        //DELETE LATER
        if(stepsLeft == 161) {
            printf("U R A MONKEY");
        }


        stepsLeft--;

        // If we're at the docking station and there are no possible next moves, return finish
        //TODO - FIND WAY TO RETURN FINISH CORRECTLY
        if(atDockingStation && newMoves.empty() && (!isGoingForward)) {
            return Step::Finish;
        }

        // If we're at the docking station, we didn't return finish, and the battery isn't full, we'll stay and charge
        if(atDockingStation && batterySensor->getBatteryState() < maxBattery) {
            return Step::Stay;
        }

        // If there's not enough battery to make a move, the vacuum can only stay in place
        if ((!atDockingStation) && batterySensor->getBatteryState() < 1) {
            return Step::Stay;
        }

        // If the battery level/ steps left are insufficient relative to the distance to the docking station, the vacuum should go back
        if (batteryInsufficient || stepsInsufficient) {
            // In case there are no steps back to perform
            if (stepsBackToDocking.empty()) {
                return Step::Stay;
            }
            isBacktracking = true;
        }

        // In case we came back to the docking station because of insufficient resources, we'll go back to the point from which we came from
        if((!stepsBackToDocking.empty()) && isBacktracking) {
            return stepBack();
        }

        // In case we came back to the docking station because of insufficient resources, we'll go back to the point from which we came from
        if((!stepsFromDocking.empty()) && isGoingForward) {
            return stepForward();
        }

        // If there's still dirt, the vacuum will stay and clean
        if(dirtSensor->dirtLevel() > 0) {
            return Step::Stay;
        }

        if(!newMoves.empty()) {
            Step nextStep = newMoves[0]; // If there are multiple smart steps available, we'll choose the first

            // Update current point
            stepsBack.push(oppositeStep(nextStep));
            logger.logInfo("Vacuum cleaner is moving forward, saving the opposite move to the backtrack path for future use");

            // Update new point
            distanceFromDock.move(nextStep);
            visitedPoints.insert(distanceFromDock);

            return nextStep;
        }
        else {
            // Go one step back, like in DFS
            return backToParent();
        }

        // If no other step is found, we'll stay in place
        logger.logInfo("No valid moves, the vacuum cleaner will stay in place");
        return Step::Stay;

    } catch (const std::exception& e) {
        logger.logError("Error deciding next step, the vacuum cleaner will stay in place. The error thrown: " + std::string(e.what()));
        return Step::Stay; // Default to staying in place on error
    }
}

void Algorithm::setMaxSteps(std::size_t maxSteps) {
    this->maxSteps = maxSteps;
    this->stepsLeft = maxSteps;
}

void Algorithm::setWallsSensor(const WallsSensor& wallsSensor) {
    this->wallsSensor = std::make_unique<WallsSensorImpl>(std::move(static_cast<const WallsSensorImpl&>(wallsSensor)));
}

void Algorithm::setDirtSensor(const DirtSensor& dirtSensor) {
    this->dirtSensor = std::make_unique<DirtSensorImpl>(std::move(static_cast<const DirtSensorImpl&>(dirtSensor)));
}

void Algorithm::setBatteryMeter(const BatteryMeter& batterySensor) {
    this->batterySensor = std::make_unique<BatteryMeterImpl>(std::move(static_cast<const BatteryMeterImpl&>(batterySensor)));
}

