#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Point.h"
#include "Sensors.h"
#include "WallsSensorImpl.h"
#include "DirtSensorImpl.h"
#include "BatteryMeterImpl.h"
#include <functional>
#include <vector>
#include <stack>

// [5]
class AbstractAlgorithm {
public:
    virtual ~AbstractAlgorithm() {}
    virtual void setMaxSteps(std::size_t maxSteps) = 0;
    virtual void setWallsSensor(const WallsSensor&) = 0;
    virtual void setDirtSensor(const DirtSensor&) = 0;
    virtual void setBatteryMeter(const BatteryMeter&) = 0;
    virtual Step nextStep() = 0;
};


class Algorithm : public AbstractAlgorithm {
public:
    Algorithm(std::function<double()> batterySensor, std::function<bool(char)> wallSensor, std::function<int(char)> dirtSensor);
    Step decideNextStep();

    void setMaxSteps(std::size_t maxSteps) override;
    void setWallsSensor(const WallsSensor&) override;
    void setDirtSensor(const DirtSensor&) override;
    void setBatteryMeter(const BatteryMeter&) override;
private:
    void calcValidMoves(std::vector<char>& moves);
    Step oppositeMove(char move) const;
    char oppositeMove(char move) const;
    
    std::function<double()> batterySensor;
    std::function<bool(char)> wallSensor;
    std::function<int(char)> dirtSensor;
    std::stack<char> stepsBack;
    std::stack<Step> stepsBack;
    Point distanceFromDock;
    bool isBacktracking;

    WallsSensor& wallsSensor;
    DirtSensor& dirtSensor;
    BatteryMeter& batterySensor;
    int maxSteps{};
    void calcValidMoves(std::vector<Step> &moves);

    Step oppositeMove(Step move) const;

};



#endif
