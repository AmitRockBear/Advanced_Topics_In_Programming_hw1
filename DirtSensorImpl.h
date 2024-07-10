#ifndef DIRTSENSOR_H
#define DIRTSENSOR_H

#include <functional>
#include "Sensors.h"

class DirtSensorImpl : public DirtSensor {
public:
    DirtSensorImpl(std::function<int()> dirtSensor);
    DirtSensorImpl(const DirtSensorImpl&& otherWallSensor);
    int dirtLevel() const override;

private:
    std::function<int()> dirtSensor;
};

#endif