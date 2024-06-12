#include "VacuumCleaner.h"
#include <stdexcept>

VacuumCleaner::VacuumCleaner(int x, int y, int maxBatterySteps) : location(Point(x, y)), battery(maxBatterySteps), maxBatterySteps(maxBatterySteps) {}

void VacuumCleaner::move(char direction) {
    int x = location.getX(), y = location.getY();

    switch (direction) {
        case 'N': location.setX(x-1); break;
        case 'E': location.setY(y+1); break;
        case 'S': location.setX(x+1); break;
        case 'W': location.setY(y-1); break;
    }

    if (!isBackTracking) {
        path.push(direction);
        allSteps.push(direction);
    }
    battery--;
}

void VacuumCleaner::increaseChargeBy(int steps) {
    battery+=steps;
    if (battery > maxBatterySteps)
        battery = maxBatterySteps;
}

void VacuumCleaner::decreaseChargeBy(int steps) {
    battery-=steps;
    if (battery < 0) {
        battery = 0;
        // Produce an error
    }
}

int VacuumCleaner::getBatteryLevel() const {
    return battery;
}

int VacuumCleaner::getMaxBatterySteps() const {
    return maxBatterySteps;
}

bool VacuumCleaner::getIsBackTracking() const {
    return isBackTracking;
}

bool VacuumCleaner::isAtLocation(int locationX, int locationY) const {
    return location.getX() == locationX && location.getY() == locationY;
}

int VacuumCleaner::getPathSize() const {
    return path.size();
}

char VacuumCleaner::backtrack() {
    if (path.empty()) {
        throw std::runtime_error("No path to backtrack");
    }
    char lastMove = path.top();
    path.pop();

    switch (lastMove) {
        case 'N': return 'S';
        case 'E': return 'W';
        case 'S': return 'N';
        case 'W': return 'E';
        default: throw std::runtime_error("Invalid move in path");
    }
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

void VacuumCleaner::flipIsBackTrackingStatus() {
    isBackTracking = !isBackTracking;
}

std::stack<char>& VacuumCleaner::getAllSteps() {
    return allSteps;
}