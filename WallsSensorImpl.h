#ifndef WALLSSENSOR_H
#define WALLSSENSOR_H

#include <functional>
#include "Sensors.h"

class WallsSensorImpl : public WallsSensor {
public:
    WallsSensorImpl(std::function<bool(Direction)> wallSensor);
    // WallsSensorImpl(const WallsSensorImpl&& otherWallSensor);
    WallsSensorImpl(const WallsSensorImpl& otherWallSensor);
    bool isWall(Direction d) const override;

private:
    std::function<bool(Direction)> wallSensor;
};

#endif