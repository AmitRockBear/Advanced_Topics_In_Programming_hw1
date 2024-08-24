#include "VacuumCleaner.h"
#include "../common/Direction.h"
#include "../common/enums.h"
#include <stdexcept>
#include <iostream>

VacuumCleaner::VacuumCleaner(ssize_t x, ssize_t y, std::size_t maxBatterySteps) : location(Point(x, y)), battery(maxBatterySteps), maxBatterySteps(maxBatterySteps) {}

void VacuumCleaner::move(Step direction) {
    location.move(direction);
}

void VacuumCleaner::increaseChargeBy(std::size_t steps) {
    double increaseBy = steps*maxBatterySteps/20.0;
    battery+=increaseBy;
    if (battery > maxBatterySteps){
        battery = maxBatterySteps;
    }
}

void VacuumCleaner::decreaseChargeBy(std::size_t steps) {
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

void VacuumCleaner::setLocation(ssize_t x, ssize_t y) {
    location.setX(x);
    location.setY(y);
}

void VacuumCleaner::initBattery(std::size_t newMaxBatterySteps) {
    battery = newMaxBatterySteps;
    maxBatterySteps = newMaxBatterySteps;
}

void VacuumCleaner::initVacuumCleaner(ssize_t x, ssize_t y, std::size_t maxBattery) {
    setLocation(x, y);
    initBattery(maxBattery);
}