#include "House.h"
#include "../common/Utils.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

House::House() {}

void House::setDockingLocation(std::size_t x, std::size_t y) {
    dockingLocation.setX(x);
    dockingLocation.setY(y);
}

void House::setHouseMap(std::vector<std::vector<int>>& houseMap) {
    this->houseMap = std::make_unique<std::vector<std::vector<int>>>(std::move(houseMap));
    totalDirt = calcTotalDirt();
}

std::size_t House::calcTotalDirt() const {
    std::size_t sum = 0;
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

std::size_t House::getTotalDirt() const {
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
    std::size_t x = location.getX();
    std::size_t y = location.getY();

    if (x >= (*houseMap).size() || y >= (*houseMap)[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    int newDirtLevel = std::max((*houseMap)[x][y] - decreaseBy, 0);
    totalDirt = totalDirt - ((*houseMap)[x][y] - newDirtLevel);
    (*houseMap)[x][y] = newDirtLevel;
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

void House::initHouse(std::vector<std::vector<int>>& houseMap, ssize_t dockingX, ssize_t dockingY) {
    setHouseMap(houseMap);
    setDockingLocation(dockingX, dockingY);
}