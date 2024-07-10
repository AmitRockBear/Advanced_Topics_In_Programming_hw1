#include "WallsSensorImpl.h"

WallsSensorImpl::WallsSensorImpl(std::function<bool(Direction)> wallSensor) : wallSensor(wallSensor) {}

WallsSensorImpl::WallsSensorImpl(const WallsSensorImpl&& other)
    : wallSensor(std::move(other.wallSensor)) {
}

bool WallsSensorImpl::isWall(Direction d) const {
    return wallSensor(d);
}
