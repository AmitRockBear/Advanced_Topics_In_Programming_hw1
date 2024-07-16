#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Point.h"
#include "dirt_sensor.h"
#include "wall_sensor.h"
#include "battery_meter.h"
#include "abstract_algorithm.h"
#include <functional>
#include <vector>
#include <stack>
#include <memory>
#include <unordered_set>
#include <unordered_map>

enum class PointState { done, wip, untouched }; // We use it for the DFS algorithm

class Algorithm : public AbstractAlgorithm {
public:
    Algorithm();
    Step nextStep() override;
    void setMaxSteps(std::size_t maxSteps) override;
    void setWallsSensor(const WallsSensor&) override;
    void setDirtSensor(const DirtSensor&) override;
    void setBatteryMeter(const BatteryMeter&) override;

    Algorithm(const Algorithm&) = delete;
    Algorithm& operator=(const Algorithm&) = delete;
    Algorithm(Algorithm&&) = delete;
    Algorithm& operator=(Algorithm&&) = delete;
private:
    Point dockingStation;
    void calcNewMoves(std::vector<Step>& newMoves);
    Step stepBack(); // Take a step back as part of the backtracking to the docking station
    Step stepForward(); // Take a forward step back as part of the path back from the docking station
    Step backToParent(); // Go back to the parent
    bool isFinished();
    size_t minStepsToUnfinishedPoint();
    std::unordered_map<Point, PointState> knownPoints; // Points we know and their status
    std::unordered_map<Point, Point> sonToParent;
    std::stack<Step> stepsBack; // Steps we've taken so far, so that when we're done with a point, we can go back to its "parent"
    std::stack<Step> stepsBackToDocking; // Steps back that combine the shortest path docking station
    std::stack<Step> stepsFromDocking; // Steps from the docking station (also, shortest path)
    Point distanceFromDock;
    const WallsSensor* wallsSensor = nullptr;
    const DirtSensor* dirtSensor = nullptr;
    const BatteryMeter* batterySensor = nullptr;
    std::stack<Step> findShortestPath(Point source, Point target);
    size_t maxSteps;
    size_t maxBattery;
    size_t stepsLeft;
    bool isBacktracking;
    bool isGoingForward;
    bool firstMove;
};

#endif
