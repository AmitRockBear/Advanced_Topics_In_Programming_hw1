#include "House.h"
#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

House::House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY)
    : houseMap(houseMap), dockingLocation(Point(dockingX, dockingY)) {
    Logger& logger = Logger::getInstance();
    
    logger.logInfo("Initializing House");

    padHouseMap(this->houseMap);
     
    totalDirt = calcTotalDirt();

    logger.logInfo("House initialized successfully with totalDirt: " + std::to_string(totalDirt));
}

int House::calcTotalDirt() const {
    int sum = 0;

    try
    {
        for (const auto& row : houseMap) {
            for (const auto& cell : row) {
                if (cell > 0) {
                    sum += cell;
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error calculating house's total dirt: " + std::string(e.what()));
    }

    return sum;
}

int House::getTotalDirt() const {
    return totalDirt;
}

int House::getDirtLevel(Point& location) const {
    int x = location.getX();
    int y = location.getY();

    if (x < 0 || x >= (int64_t)houseMap.size() || y < 0 || y >= (int64_t)houseMap[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    return houseMap[x][y];
}

void House::decreaseDirtLevel(Point& location, int decreaseBy) {
    Logger& logger = Logger::getInstance();

    logger.logInfo("Decreasing dirt level by: " + std::to_string(decreaseBy) + " at location: " + location.toString());

    int x = location.getX();
    int y = location.getY();

    if (x < 0 || x >= (int64_t)houseMap.size() || y < 0 || y >= (int64_t)houseMap[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    int newDirtLevel = std::max(houseMap[x][y] - decreaseBy, 0); // In case decreaseBy given is too big
    totalDirt = totalDirt - (houseMap[x][y] - newDirtLevel);
    houseMap[x][y] = newDirtLevel;

    logger.logInfo("Dirt level at location: " + location.toString() + " is now: " + std::to_string(newDirtLevel) + " Total dirt in house: " + std::to_string(totalDirt));
}

bool House::isWall(Point& location) const {
    int x = location.getX();
    int y = location.getY();

    if (x < 0 || x >= (int64_t)houseMap.size() || y < 0 || y >= (int64_t)houseMap[0].size()) {
        return true;
    }

    return houseMap[x][y] == -1;
}

void House::getDockingLocation(Point& locationToModify) const {
    locationToModify.setX(dockingLocation.getX());
    locationToModify.setY(dockingLocation.getY());
}

size_t House::getMaxY(std::vector<std::vector<int>>& houseMap) {
    size_t maxRowSize = 0;
    for (const auto& row : houseMap) {
        if (row.size() > maxRowSize) {
            maxRowSize = row.size();
        }
    }
    return maxRowSize;
}

void House::houseVisualization(Point vacuumLocation) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(WAITING_TIME));
    clear();
    for (int i = 0; i < (int64_t)houseMap.size(); i++) {
        for (int j = 0; j < (int64_t)houseMap[i].size(); j++) {
            int cell = houseMap[i][j];
            if(cell == -1) {
                if (i == 0 || i == (int64_t)houseMap.size() - 1) {
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

void House::padHouseMap(std::vector<std::vector<int>>& houseMap) {
    try
    {
        size_t maxY = getMaxY(houseMap);
        for (auto& row : houseMap) {
            while (row.size() < maxY) {
                row.push_back(-1);
            }
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error padding house map: " + std::string(e.what()));
    }
}