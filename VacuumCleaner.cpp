#include "VacuumCleaner.h"
#include "Logger.h"
#include <stdexcept>
#include <iostream>

VacuumCleaner::VacuumCleaner(int x, int y, int maxBatterySteps) : location(Point(x, y)), battery(maxBatterySteps), maxBatterySteps(maxBatterySteps) {
    Logger& logger = Logger::getInstance();
    
    logger.logInfo("VacuumCleaner successfully initialized at location: " + location.toString() + " with maxBatterySteps: " + std::to_string(maxBatterySteps));
}

void VacuumCleaner::move(char direction) {
    Logger::getInstance().logInfo("Moving vacuum cleaner to direction: " + std::string(1, direction));
    location.move(direction);
    Logger::getInstance().logInfo("Successfully moved vacuum cleaner to location: " + location.toString());
}

void VacuumCleaner::increaseChargeBy(int steps) {
    double increaseBy = steps*maxBatterySteps/20.0;

    Logger::getInstance().logInfo("Increasing battery level by: " + std::to_string(increaseBy));
    
    battery+=increaseBy;
    if (battery > maxBatterySteps){
        Logger::getInstance().logInfo("Battery level surpassed maxBatterySteps, adjusting to maxBatterySteps");
        battery = maxBatterySteps;
    }
}

void VacuumCleaner::decreaseChargeBy(int steps) {
    Logger::getInstance().logInfo("Decreasing battery level by: " + std::to_string(steps));
    battery-=steps;
    if (battery < 0) {
        battery = 0;
    }
}

double VacuumCleaner::getBatteryLevel() const {
    return battery;
}

int VacuumCleaner::getMaxBatterySteps() const {
    return maxBatterySteps;
}

bool VacuumCleaner::isAtLocation(Point& otherLocation) const {
    return location == otherLocation;
}

int VacuumCleaner::getX() const {
    return location.getX();
}

int VacuumCleaner::getY() const {
    return location.getY();
}

void VacuumCleaner::getLocation(Point& locationToModify) const {
    locationToModify.setX(location.getX());
    locationToModify.setY(location.getY());
}