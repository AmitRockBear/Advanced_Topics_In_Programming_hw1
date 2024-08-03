#ifndef WALLSSENSORIMPL_H
#define WALLSSENSORIMPL_H

#include <functional>
#include "../simulator/wall_sensor.h"

class WallsSensorImpl : public WallsSensor {
public:
    WallsSensorImpl(std::function<bool(Direction)> wallSensor);
    bool isWall(Direction d) const override;

private:
    std::function<bool(Direction)> wallSensor;
};

#endif