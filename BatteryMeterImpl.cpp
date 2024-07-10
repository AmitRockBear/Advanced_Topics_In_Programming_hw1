#include "BatteryMeterImpl.h"

BatteryMeterImpl::BatteryMeterImpl(std::function<std::size_t()> batterySensor) : batterySensor(batterySensor) {}

BatteryMeterImpl::BatteryMeterImpl(const BatteryMeterImpl&& other)
    : batterySensor(std::move(other.batterySensor)) {
}

std::size_t BatteryMeterImpl::getBatteryState() const {
    return batterySensor();
}
