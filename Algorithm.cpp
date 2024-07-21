#include "Algorithm.h"
#include "General.h"
#include "Logger.h"
#include "BatteryMeterImpl.h"
#include "DirtSensorImpl.h"
#include "WallsSensorImpl.h"
#include "Direction.h"
#include <ctime>
#include <queue>

Algorithm::Algorithm(): dockingStation(Point(0ll,0ll)), distanceFromDock(dockingStation), maxSteps(0), maxBattery(0), stepsLeft(0), isBacktracking(false), isGoingForward(false), firstMove(true) {
    knownPoints[dockingStation] = PointState::wip;
    sonToParent[dockingStation] = dockingStation;
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
    if(stepsFromDocking.empty()) {
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
    bool parentExists = sonToParent.contains(distanceFromDock);
    Point parent;

    // We don't want to check the parent, because by DFS definition it should be wip and not done
    if(parentExists) {
        parent = sonToParent[distanceFromDock];
    }

    for (auto &&direction : DIRECTIONS) {
        neighbor = distanceFromDock.getNeighbor(direction);
        isWall = wallsSensor->isWall(direction);
        isDockingStation = (neighbor == dockingStation);

        if(isWall || (parentExists && parent == neighbor)) {
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

// Returns number of steps to from the docking station to the closest point that has wip/untouched status
size_t Algorithm::minStepsToUnfinishedPoint() {
    size_t currSteps;
    size_t minSteps = maxSteps;
    Point point;
    PointState state;
    //for(auto it = knownPoints.begin(); it != knownPoints.end(); ++it) {
    for(auto & knownPoint : knownPoints) {
        point = knownPoint.first;
        state = knownPoint.second;
        if(point == dockingStation) {
            continue;
        }
        if(state != PointState::done) {
            currSteps = findShortestPath(dockingStation, point).size();
            if(currSteps < minSteps) {
                minSteps = currSteps;
            }
        }
    }
    return minSteps;
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
    while (current != source) {
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
        bool isDirty = dirtSensor->dirtLevel() > 0;

        if(stepsLeft == 0) {
            return Step::Finish;
        }

        // Check if all other neighbors, beside parent, are done
        if(isFinished()) {
            // If the point is clean and finished, we'll update its status
            if((!atDockingStation) && (!isDirty)) {
                knownPoints[distanceFromDock] = PointState::done;
            }
            // If we're at the docking station and finished, we'll return Finish
            else if (atDockingStation) {
                return Step::Finish;
            }
        }

        // If we're back at the docking station, and not finished, we'd like to empty the backtrack path
        if (atDockingStation) {
            if(!stepsBackToDocking.empty()) {
                logger.logInfo("Back at the docking station, clearing the backtrack path");
                stepsBackToDocking = std::stack<Step>();
            }
            if (isBacktracking) {
                isGoingForward = true;
            }
            isBacktracking = false;

            // We'll also make sure we have enough steps to go on. Otherwise, we'll finish
            size_t minSteps = minStepsToUnfinishedPoint();
            if(stepsLeft < minSteps || maxBattery < minSteps)
                return Step::Finish;
        }
        else {
            // Update steps back based on current shortest path - We check it everytime because the path is based on the points we've been to so far
            stepsBackToDocking = findShortestPath(distanceFromDock, dockingStation);
        }

        std::vector<Step> newMoves = std::vector<Step>(); // newMoves = Moves to neighboring points we haven't visited yet
        calcNewMoves(newMoves);

        size_t stepsToDockingStation = stepsBackToDocking.size();
        //bool batteryInsufficient = (batterySensor->getBatteryState() >= stepsToDockingStation && batterySensor->getBatteryState() <= stepsToDockingStation + 1);
        bool batteryInsufficient = ((batterySensor->getBatteryState() < stepsToDockingStation + 1) || (batterySensor->getBatteryState() == stepsToDockingStation + 1 && (!isDirty)));
        bool stepsInsufficient = (stepsToDockingStation >= stepsLeft);

        stepsLeft--;

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
        if(isDirty) {
            return Step::Stay;
        }

        if(!newMoves.empty()) {
            Step nextStep = newMoves[0]; // If there are multiple smart steps available, we'll choose the first
            sonToParent[distanceFromDock.getNeighbor(MovableStepToDirection(nextStep))] = distanceFromDock; // Current point is the parent of new point

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
    this->wallsSensor = &wallsSensor; 
}

void Algorithm::setDirtSensor(const DirtSensor& dirtSensor) {
    this->dirtSensor = &dirtSensor;
}

void Algorithm::setBatteryMeter(const BatteryMeter& batterySensor) {
    this->batterySensor = &batterySensor;
}

