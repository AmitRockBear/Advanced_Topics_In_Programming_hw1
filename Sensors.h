#ifndef SENSORS_H
#define SENSORS_H

#include "Direction.h"

class WallsSensor {
public:
    virtual ~WallsSensor() {}
    virtual bool isWall(Direction d) const = 0;
};

class DirtSensor {
public:
    virtual ~DirtSensor() {}
    virtual int dirtLevel() const = 0;
};

class BatteryMeter {
public:
    virtual ~BatteryMeter() {}
    virtual std::size_t getBatteryState() const = 0;
};
#endif
