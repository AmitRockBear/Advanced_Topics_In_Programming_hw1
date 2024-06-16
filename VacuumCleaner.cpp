#include "VacuumCleaner.h"
#include <stdexcept>
#include <iostream>

VacuumCleaner::VacuumCleaner(int x, int y, int maxBatterySteps) : location(Point(x, y)), battery(maxBatterySteps), maxBatterySteps(maxBatterySteps) {}

void VacuumCleaner::move(char direction) {
    int x = location.getX(), y = location.getY();

    switch (direction) {
        case 'N': setX(x-1); break;
        case 'E': setY(y+1); break;
        case 'S': setX(x+1); break;
        case 'W': setY(y-1); break;
        default: break;
    }
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

bool VacuumCleaner::isAtLocation(int locationX, int locationY) const {
    return location.getX() == locationX && location.getY() == locationY;
}

int VacuumCleaner::getX() const {
    return location.getX();
}

int VacuumCleaner::getY() const {
    return location.getY();
}

void VacuumCleaner::setX(int newX) {
    location.setX(newX);
}

void VacuumCleaner::setY(int newY) {
    location.setY(newY);
}
