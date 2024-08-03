#ifndef BATTERYMETERIMPL_H
#define BATTERYMETERIMPL_H

#include "../simulator/battery_meter.h"
#include <functional>

class BatteryMeterImpl : public BatteryMeter {
public:
    BatteryMeterImpl(std::function<std::size_t()> batterySensor);
    std::size_t getBatteryState() const override;
private:
    std::function<std::size_t()> batterySensor;
};

#endif