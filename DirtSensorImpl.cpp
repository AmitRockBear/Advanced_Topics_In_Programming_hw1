#include "DirtSensorImpl.h"

DirtSensorImpl::DirtSensorImpl(std::function<int()> dirtSensor) : dirtSensor(dirtSensor) {}

// DirtSensorImpl::DirtSensorImpl(const DirtSensorImpl&& otherDirtSensor) {
//     dirtSensor = otherDirtSensor.dirtSensor;
//     // otherDirtSensor.dirtSensor = nullptr;
// }

DirtSensorImpl::DirtSensorImpl(const DirtSensorImpl& otherDirtSensor) {
    dirtSensor = otherDirtSensor.dirtSensor;
}

DirtSensorImpl& DirtSensorImpl::operator=(const DirtSensorImpl& other) {
    if (this != &other) {  // Self-assignment check
        // Copy data members from 'other' to 'this'
        dirtSensor = other.dirtSensor;
    }
    return *this;
}

int DirtSensorImpl::dirtLevel() const {
    return dirtSensor();
}
