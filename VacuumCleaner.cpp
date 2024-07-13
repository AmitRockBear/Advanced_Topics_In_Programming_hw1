#include "VacuumCleaner.h"
#include "Logger.h"
#include "Direction.h"
#include <stdexcept>
#include <iostream>

VacuumCleaner::VacuumCleaner(ssize_t x, ssize_t y, std::size_t maxBatterySteps) : location(Point(x, y)), battery(maxBatterySteps), maxBatterySteps(maxBatterySteps) {
    Logger& logger = Logger::getInstance();
    
    logger.logInfo("VacuumCleaner successfully initialized at location: " + location.toString() + " with maxBatterySteps: " + std::to_string(maxBatterySteps));
}

void VacuumCleaner::move(Step direction) {
    location.move(direction);
}

void VacuumCleaner::increaseChargeBy(std::size_t steps) {
    Logger& logger = Logger::getInstance();
    double increaseBy = steps*maxBatterySteps/20.0;

    logger.logInfo("Increasing battery level by: " + std::to_string(increaseBy));
    
    battery+=increaseBy;
    if (battery > maxBatterySteps){
        logger.logInfo("Battery level surpassed maxBatterySteps, adjusting to maxBatterySteps");
        battery = maxBatterySteps;
    }
}

void VacuumCleaner::decreaseChargeBy(std::size_t steps) {
    Logger::getInstance().logInfo("Decreasing battery level by: " + std::to_string(steps));
    battery-=steps;
    if (battery < 0) {
        battery = 0;
    }
}

double VacuumCleaner::getBatteryLevel() const {
    return battery;
}

std::size_t VacuumCleaner::getMaxBatterySteps() const {
    return maxBatterySteps;
}

bool VacuumCleaner::isAtLocation(Point& otherLocation) const {
    return location == otherLocation;
}

ssize_t VacuumCleaner::getX() const {
    return location.getX();
}

ssize_t VacuumCleaner::getY() const {
    return location.getY();
}

void VacuumCleaner::getLocation(Point& locationToModify) const {
    locationToModify.setX(location.getX());
    locationToModify.setY(location.getY());
}