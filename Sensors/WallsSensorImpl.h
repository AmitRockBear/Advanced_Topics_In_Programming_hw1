#ifndef WALLSSENSOR_H
#define WALLSSENSOR_H

#include <functional>
#include "skeleton.h"

class WallsSensorImpl : public WallsSensor {
public:
    WallsSensorImpl(std::function<bool(Direction)> wallSensor);
    bool isWall(Direction d) const override;

private:
    std::function<bool(Direction)> wallSensor;
};

#endif