#include "BatteryMeterImpl.h"

BatteryMeterImpl::BatteryMeterImpl(std::function<std::size_t()> batterySensor) : batterySensor(batterySensor) {}

// BatteryMeterImpl::BatteryMeterImpl(const BatteryMeterImpl&& otherBatteryMeter) {
//     batterySensor = otherBatteryMeter.batterySensor;
//     // otherBatteryMeter.batterySensor = nullptr;
// }

BatteryMeterImpl::BatteryMeterImpl(const BatteryMeterImpl& otherBatteryMeter) {
    batterySensor = otherBatteryMeter.batterySensor;
}

std::size_t BatteryMeterImpl::getBatteryState() const {
    return batterySensor();
}
