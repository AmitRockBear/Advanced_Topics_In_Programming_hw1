#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Point.h"
#include "Sensors.h"
#include <functional>
#include <vector>
#include <stack>
#include <memory>

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
    Algorithm();
    Step nextStep() override;

    void setMaxSteps(std::size_t maxSteps) override;
    void setWallsSensor(const WallsSensor&) override;
    void setDirtSensor(const DirtSensor&) override;
    void setBatteryMeter(const BatteryMeter&) override;
private:
    void calcValidMoves(std::vector<char>& moves);
    Step oppositeMove(Step move) const;
    
    std::stack<Step> stepsBack;
    Point distanceFromDock;
    bool isBacktracking;

    std::unique_ptr<WallsSensor> wallsSensor;
    std::unique_ptr<DirtSensor> dirtSensor;
    std::unique_ptr<BatteryMeter> batterySensor;
    int maxSteps{};
    void calcValidMoves(std::vector<Step> &moves);
};

#endif
