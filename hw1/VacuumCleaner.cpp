#include "VacuumCleaner.h"
#include <stdexcept>

VacuumCleaner::VacuumCleaner(House* house) : house(house), stepsTaken(0), missionCompleted(false), dead(false) {
    x = house->getDockingX();
    y = house->getDockingY();
    battery = house->getMaxBatterySteps();
}

void VacuumCleaner::move(char direction) {
    if (battery <= 0) {
        dead = true;
        return;
    }

    int newX = x, newY = y;

    switch (direction) {
        case 'N': newX--; break;
        case 'E': newY++; break;
        case 'S': newX++; break;
        case 'W': newY--; break;
        case 'S': break; // Stay in place
        default: throw std::invalid_argument("Invalid direction");
    }

    if (house->isWall(newX, newY)) {
        return; // Do not move into walls
    }

    if (direction != 'S') {
        x = newX;
        y = newY;
        path.push(direction);
    }

    battery--;
    stepsTaken++;

    updateMissionStatus();
}

void VacuumCleaner::clean() {
    int dirtLevel = house->getDirtLevel(x, y);
    if (dirtLevel > 0) {
        house->setDirtLevel(x, y, dirtLevel - 1);
    }
    updateMissionStatus();
}

void VacuumCleaner::charge(int steps) {
    if (isAtDockingStation()) {
        battery += steps * (house->getMaxBatterySteps() / 20);
        if (battery > house->getMaxBatterySteps()) {
            battery = house->getMaxBatterySteps();
        }
    }
}

bool VacuumCleaner::isMissionCompleted() const {
    return missionCompleted;
}

bool VacuumCleaner::isDead() const {
    return dead;
}

int VacuumCleaner::getStepsTaken() const {
    return stepsTaken;
}

int VacuumCleaner::getBatteryLevel() const {
    return battery;
}

int VacuumCleaner::getMaxBatterySteps() const {
    return house->getMaxBatterySteps();
}

bool VacuumCleaner::isAtDockingStation() const {
    return x == house->getDockingX() && y == house->getDockingY();
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

int VacuumCleaner::getDirtLevel() const {
    return house->getDirtLevel(x, y);
}

bool VacuumCleaner::isWall(char direction) const {
    int newX = x, newY = y;

    switch (direction) {
        case 'N': newX--; break;
        case 'E': newY++; break;
        case 'S': newX++; break;
        case 'W': newY--; break;
        default: throw std::invalid_argument("Invalid direction");
    }

    return house->isWall(newX, newY);
}

void VacuumCleaner::updateMissionStatus() {
    if (house->getTotalDirt() == 0 && isAtDockingStation()) {
        missionCompleted = true;
    }
}
