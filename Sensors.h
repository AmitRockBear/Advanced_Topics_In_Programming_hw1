//
// Created by Michal on 09/07/2024.
//

#ifndef SENSORS_H
#define SENSORS_H
#include "Direction.h"
// [2]
class WallsSensor {
public:
    virtual ~WallsSensor() {}
    virtual bool isWall(Direction d) const = 0;
};

// [3]
class DirtSensor {
public:
    virtual ~DirtSensor() {}
    virtual int dirtLevel() const = 0;
};

// [4]
class BatteryMeter {
public:
    virtual ~BatteryMeter() {}
    virtual std::size_t getBatteryState() const = 0;
};
#endif //SENSORS_H
