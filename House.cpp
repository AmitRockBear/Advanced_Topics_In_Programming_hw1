#include "House.h"
#include <fstream>
#include <iostream>

House::House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY)
    : houseMap(houseMap), dockingLocation(Point(dockingX, dockingY)) {
    padHouseMap(this->houseMap);
    totalDirt = calcTotalDirt();
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

    if (x < 0 || x >= houseMap.size() || y < 0 || y >= houseMap[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    return houseMap[x][y];
}

void House::decreaseDirtLevel(Point& location, int decreaseBy) {
    int x = location.getX();
    int y = location.getY();
    
    if (x < 0 || x >= houseMap.size() || y < 0 || y >= houseMap[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    int newDirtLevel = std::max(houseMap[x][y] - decreaseBy, 0); // In case decreaseBy given is too big
    totalDirt = totalDirt - (houseMap[x][y] - newDirtLevel);
    houseMap[x][y] = newDirtLevel;
}

bool House::isWall(Point& location) const {
    int x = location.getX();
    int y = location.getY();

    if (x < 0 || x >= houseMap.size() || y < 0 || y >= houseMap[0].size()) {
        return true;
    }
    
    return houseMap[x][y] == -1;
}


int House::getDockingX() const {
    return dockingLocation.getX();
}

int House::getDockingY() const {
    return dockingLocation.getY();
}

void House::getDockingLocation(Point& locationToModify) const {
    locationToModify.setX(dockingLocation.getX());
    locationToModify.setY(dockingLocation.getY());
}

void House::printHouse() const {
    std::cout << "House:" << std::endl;
    for (const auto &row: houseMap) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
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