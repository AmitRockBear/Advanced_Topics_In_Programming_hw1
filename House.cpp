#include "House.h"
#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

House::House() {
    Logger::getInstance().logInfo("Initializing House");
}

void House::setDockingLocation(std::size_t x, std::size_t y) {
    Logger::getInstance().logInfo("Setting docking location");
    dockingLocation.setX(x);
    dockingLocation.setY(y);
}

void House::setHouseMap(std::vector<std::vector<int>>& houseMap) {
    Logger::getInstance().logInfo("Setting new house map and calculating total dirt");
    this->houseMap = std::make_unique<std::vector<std::vector<int>>>(std::move(houseMap));
    totalDirt = calcTotalDirt();
}

int House::calcTotalDirt() const {
    int sum = 0;

    try {
        for (const auto& row : (*houseMap)) {
            for (const auto& cell : row) {
                if (cell > 0) {
                    sum += cell;
                }
            }
        }
    }
    catch(const std::exception& e) {
        throw std::runtime_error("Error calculating house's total dirt: " + std::string(e.what()));
    }

    return sum;
}

int House::getTotalDirt() const {
    return totalDirt;
}

int House::getDirtLevel(Point& location) const {
    std::size_t x = location.getX();
    std::size_t y = location.getY();

    if (x >= (*houseMap).size() || y >= (*houseMap)[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    return (*houseMap)[x][y];
}

void House::decreaseDirtLevel(Point& location, int decreaseBy) {
    Logger& logger = Logger::getInstance();

    logger.logInfo("Decreasing dirt level by: " + std::to_string(decreaseBy) + " at location: " + location.toString());

    std::size_t x = location.getX();
    std::size_t y = location.getY();

    if (x >= (*houseMap).size() || y >= (*houseMap)[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    int newDirtLevel = std::max((*houseMap)[x][y] - decreaseBy, 0);
    totalDirt = totalDirt - ((*houseMap)[x][y] - newDirtLevel);
    (*houseMap)[x][y] = newDirtLevel;

    logger.logInfo("Dirt level at location: " + location.toString() + " is now: " + std::to_string(newDirtLevel) + " Total dirt in house: " + std::to_string(totalDirt));
}

bool House::isWall(Point& location) const {
    ssize_t x = location.getX();
    ssize_t y = location.getY();

    if (x < 0 || x >= (ssize_t)(*houseMap).size() || y < 0 || y >= (ssize_t)(*houseMap)[0].size()) {
        return true;
    }

    return (*houseMap)[x][y] == -1;
}

void House::getDockingLocation(Point& locationToModify) const {
    locationToModify.setX(dockingLocation.getX());
    locationToModify.setY(dockingLocation.getY());
}

void House::houseVisualization(Point vacuumLocation) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(WAITING_TIME));
    clear();
    for (ssize_t i = 0; i < (ssize_t)(*houseMap).size(); i++) {
        for (ssize_t j = 0; j < (ssize_t)(*houseMap)[i].size(); j++) {
            int cell = (*houseMap)[i][j];
            if(cell == -1) {
                if (i == 0 || i == (ssize_t)(*houseMap).size() - 1) {
                    std::cout << HORIZONTAL_WALL << " ";
                }
                else {
                    std::cout << VERTICAL_WALL << " ";
                }
            }
            else if (i == vacuumLocation.getX() && j == vacuumLocation.getY()) {
                std::cout << VACUUM_SIGN << " ";
            }
            else if (i == dockingLocation.getX() && j == dockingLocation.getY()) {
                std::cout << DOCKING_SIGN << " ";
            }
            else {
                std::cout << cell << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void House::initHouse(std::vector<std::vector<int>>& houseMap, ssize_t dockingX, ssize_t dockingY) {
    setHouseMap(houseMap);
    setDockingLocation(dockingX, dockingY);
}