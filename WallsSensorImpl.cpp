#include "WallsSensorImpl.h"

WallsSensorImpl::WallsSensorImpl(std::function<bool(Direction)> wallSensor) : wallSensor(wallSensor) {}

// WallsSensorImpl::WallsSensorImpl(const WallsSensorImpl&& otherWallSensor) {
//     wallSensor = otherWallSensor.wallSensor;
//     // otherWallSensor.wallSensor = nullptr;
// }

WallsSensorImpl::WallsSensorImpl(const WallsSensorImpl& otherWallSensor) {
    wallSensor = otherWallSensor.wallSensor;
}

bool WallsSensorImpl::isWall(Direction d) const {
    return wallSensor(d);
}
