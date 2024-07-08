#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Point.h"
#include "skeleton.h"
#include "Sensors/WallsSensorImpl.h"
#include "Sensors/DirtSensorImpl.h"
#include "Sensors/BatteryMeterImpl.h"
#include <functional>
#include <vector>
#include <stack>

class Algorithm : public AbstractAlgorithm {
public:
    Algorithm(std::function<double()> batterySensor, std::function<bool(char)> wallSensor, std::function<int(char)> dirtSensor);
    char decideNextStep();

    // Algorithm();
    // void setMaxSteps(std::size_t maxSteps) override;
    // void setWallsSensor(const WallsSensor&) override;
    // void setDirtSensor(const DirtSensor&) override;
    // void setBatteryMeter(const BatteryMeter&) override;
private:
    void calcValidMoves(std::vector<char>& moves);
    char oppositeMove(char move) const;
    
    std::function<double()> batterySensor;
    std::function<bool(char)> wallSensor;
    std::function<int(char)> dirtSensor;
    std::stack<char> stepsBack;
    Point distanceFromDock;
    bool isBacktracking;

    // WallsSensor& wallsSensor;
    // DirtSensor& dirtSensor;
    // BatteryMeter& batterySensor;
    // int maxSteps;
};

#endif
