#include "DirtSensorImpl.h"

DirtSensorImpl::DirtSensorImpl(std::function<int()> dirtSensor) : dirtSensor(dirtSensor) {}

DirtSensorImpl::DirtSensorImpl(const DirtSensorImpl&& other)
    : dirtSensor(std::move(other.dirtSensor)) {
}

int DirtSensorImpl::dirtLevel() const {
    return dirtSensor();
}
