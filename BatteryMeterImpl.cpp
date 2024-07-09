#include "BatteryMeterImpl.h"


BatteryMeterImpl::BatteryMeterImpl(std::function<std::size_t()> batterySensor) : batterySensor(batterySensor) {}

std::size_t BatteryMeterImpl::getBatteryState() const {
    return batterySensor();
}
