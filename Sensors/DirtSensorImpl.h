#ifndef DIRTSENSOR_H
#define DIRTSENSOR_H

#include <functional>
#include "skeleton.h"

class DirtSensorImpl : public DirtSensor {
public:
    DirtSensorImpl(std::function<int()> dirtSensor);
    int dirtLevel() const override;

private:
    std::function<int()> dirtSensor;
};

#endif