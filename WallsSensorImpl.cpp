#include "WallsSensorImpl.h"

WallsSensorImpl::WallsSensorImpl(std::function<bool(Direction)> wallSensor) : wallSensor(wallSensor) {}

bool WallsSensorImpl::isWall(Direction d) const {
    return wallSensor(d);
}
