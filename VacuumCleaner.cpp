#include "VacuumCleaner.h"
#include <stdexcept>
#include <iostream>

VacuumCleaner::VacuumCleaner(int x, int y, int maxBatterySteps) : location(Point(x, y)), battery(maxBatterySteps), maxBatterySteps(maxBatterySteps) {}

void VacuumCleaner::move(char direction) {
    location.move(direction);
}

void VacuumCleaner::increaseChargeBy(int steps) {
    battery+=steps*maxBatterySteps/20.0;
    if (battery > maxBatterySteps)
        battery = maxBatterySteps;
}

void VacuumCleaner::decreaseChargeBy(int steps) {
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

Point VacuumCleaner::getLocation() const {
    return location;
}