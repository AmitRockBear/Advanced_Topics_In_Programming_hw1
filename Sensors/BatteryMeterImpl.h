#ifndef BATTERYMETER_H
#define BATTERYMETER_H

#include <functional>
#include "skeleton.h"

class BatteryMeterImpl : public BatteryMeter {
public:
    BatteryMeterImpl(std::function<std::size_t()> batterySensor);
    std::size_t getBatteryState() const override;
private:
    std::function<std::size_t()> batterySensor;
};

#endif