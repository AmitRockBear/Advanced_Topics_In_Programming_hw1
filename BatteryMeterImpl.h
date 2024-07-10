#ifndef BATTERYMETER_H
#define BATTERYMETER_H

#include <functional>
#include "Sensors.h"

class BatteryMeterImpl : public BatteryMeter {
public:
    BatteryMeterImpl(std::function<std::size_t()> batterySensor);
    // BatteryMeterImpl(const BatteryMeterImpl&& otherBatteryMeter);
    BatteryMeterImpl(const BatteryMeterImpl& otherBatteryMeter);
    
    std::size_t getBatteryState() const override;
private:
    std::function<std::size_t()> batterySensor;
};

#endif