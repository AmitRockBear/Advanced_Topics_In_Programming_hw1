#include "DirtSensorImpl.h"

DirtSensorImpl::DirtSensorImpl(std::function<int()> dirtSensor) : dirtSensor(dirtSensor) {}

int DirtSensorImpl::dirtLevel() const {
    return dirtSensor();
}
