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

Algorithm::Algorithm(): dockingStation(Point(0ll,0ll)), distanceFromDock(dockingStation), maxSteps(0), maxBattery(0), stepsLeft(0), isBacktracking(false), isGoingForward(false), firstMove(true) {
    knownPoints[dockingStation] = PointState::wip;
}

// Fills the vector newMoves with all possible moves to neighbor points that we haven't visited yet
void Algorithm::calcNewMoves(std::vector<Step>& newMoves) {
    bool isWall, notVisited;
    Point neighbor;
    for (auto &&direction : DIRECTIONS) {
        neighbor = distanceFromDock.getNeighbor(direction);
        isWall = wallsSensor->isWall(direction);
        notVisited = ((!knownPoints.contains(neighbor)) || (knownPoints[neighbor] == PointState::untouched));
        if((!isWall) && notVisited) {
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

// Checks if we're finished with the point. True iff we're finished with all neighbor points
bool Algorithm::isFinished() {
    bool isWall;
    bool isDockingStation;
    Point neighbor;
    bool parentExists = !stepsBack.empty();
    Direction parentDirection;

    // We don't want to check the parent, because by DFS definition it should be wip and not done
    if(parentExists) {
        parentDirection = MovableStepToDirection(stepsBack.top());
    }

    for (auto &&direction : DIRECTIONS) {
        neighbor = distanceFromDock.getNeighbor(direction);
        isWall = wallsSensor->isWall(direction);
        isDockingStation = (neighbor == dockingStation);

        if(isWall ||  (parentExists && direction == parentDirection)) {
            continue;
        }
        // Add neighbor to knownPoints if it isn't already there
        if(!knownPoints.contains(neighbor)) {
            knownPoints[neighbor] = PointState::untouched;
        }

        // If there's even one neighbor that's not done, the point is not finished
        if((!isDockingStation) && knownPoints[neighbor] != PointState::done) {
            return false;
        }
    }
    return true;
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

        //DELETE LATER
        if(stepsLeft == 160) {
            printf("Got here");
        }

        if(stepsLeft == 0) {
            return Step::Finish;
        }

        // Check if all other neighbors, beside parent, are done
        if(isFinished()) {
            // If the point is clean and finished, we'll update its status
            if((!atDockingStation) && dirtSensor->dirtLevel() == 0) {
                knownPoints[distanceFromDock] = PointState::done;
            }
            // If we're at the docking station and finished, we'll return Finish
            else if (atDockingStation) {
                return Step::Finish;
            }
        }

        // If we're back at the docking station, and not finished, we'd like to empty the backtrack path
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

        std::vector<Step> newMoves = std::vector<Step>(); // newMoves = Moves to neighboring points we haven't visited yet
        calcNewMoves(newMoves);

        size_t stepsToDockingStation = stepsBackToDocking.size();
        bool batteryInsufficient = (batterySensor->getBatteryState() >= stepsToDockingStation && batterySensor->getBatteryState() <= stepsToDockingStation + 1);
        bool stepsInsufficient = (stepsToDockingStation >= stepsLeft);

        //DELETE LATER
        logger.logInfo("Point relative to the docking station: " + distanceFromDock.toString());
        logger.logInfo("Steps left: " +   std::to_string(stepsLeft));


        stepsLeft--;

        // If we're at the docking station and there are no possible next moves, return finish
//        if(atDockingStation && newMoves.empty() && (!isGoingForward)) {
//            return Step::Finish;
//        }

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

            // Add opposite step to the DFS path for later use
            stepsBack.push(oppositeStep(nextStep));
            logger.logInfo("Vacuum cleaner is moving forward, saving the opposite move to the backtrack path for future use");

            // Update new point
            distanceFromDock.move(nextStep);
            knownPoints[distanceFromDock] = PointState::wip;

            return nextStep;
        }
        else {
            // We're done with the current point, so we go one step back to its "parent", like in DFS
            knownPoints[distanceFromDock] = PointState::done;
            if (!stepsBack.empty()) {
                return backToParent();
            }
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

