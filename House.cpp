#include "House.h"
#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

House::House(std::vector<std::vector<std::size_t>>& houseMap, std::size_t dockingX, std::size_t dockingY)
    : houseMap(std::make_unique<std::vector<std::vector<std::size_t>>>(std::move(houseMap))), dockingLocation(Point(dockingX, dockingY)) {
    Logger& logger = Logger::getInstance();
    
    logger.logInfo("Initializing House");

    totalDirt = calcTotalDirt();

    logger.logInfo("House initialized successfully with totalDirt: " + std::to_string(totalDirt));
}

House::House() : houseMap(std::make_unique<std::vector<std::vector<std::size_t>>>()), dockingLocation(Point(0, 0)), totalDirt(0) {}

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

    int newDirtLevel = (*houseMap)[x][y];
    if ((*houseMap)[x][y] - decreaseBy < 0)
        newDirtLevel = 0;

    totalDirt = totalDirt - ((*houseMap)[x][y] - newDirtLevel);
    (*houseMap)[x][y] = newDirtLevel;

    logger.logInfo("Dirt level at location: " + location.toString() + " is now: " + std::to_string(newDirtLevel) + " Total dirt in house: " + std::to_string(totalDirt));
}

bool House::isWall(Point& location) const {
    std::size_t x = location.getX();
    std::size_t y = location.getY();

    if (x >= (*houseMap).size() || y >= (*houseMap)[0].size()) {
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
    for (std::size_t i = 0; i < (*houseMap).size(); i++) {
        for (std::size_t j = 0; j < (*houseMap)[i].size(); j++) {
            int cell = (*houseMap)[i][j];
            if(cell == -1) {
                if (i == 0 || i == (*houseMap).size() - 1) {
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