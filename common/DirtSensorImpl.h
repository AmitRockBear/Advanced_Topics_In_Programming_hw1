#ifndef DIRTSENSORIMPL_H
#define DIRTSENSORIMPL_H

#include <functional>
#include "../simulator/dirt_sensor.h"

class DirtSensorImpl : public DirtSensor {
public:
    DirtSensorImpl(std::function<int()> dirtSensor);
    int dirtLevel() const override;

private:
    std::function<int()> dirtSensor;
};

#endif