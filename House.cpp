#include "House.h"
#include <fstream>
#include <iostream>

House::House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY)
    : houseMap(houseMap), dockingLocation(Point(dockingX, dockingY))
    {
        totalDirt = calcTotalDirt(); // In order for it to be called after initializing the houseMap
    }

int House::calcTotalDirt() const {
    int sum = 0;
    for (const auto& row : houseMap) {
        for (const auto& cell : row) {
            if (cell > 0) {
                sum += cell;
            }
        }
    }
    return sum;
}

int House::getTotalDirt() const {
    return totalDirt;
}

int House::getDirtLevel(int x, int y) const {
    if (x < 0 || x >= houseMap.size() || y < 0 || y >= houseMap[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }
    return houseMap[x][y];
}

void House::decreaseDirtLevel(int x, int y, int decreaseBy) {
    if (x < 0 || x >= houseMap.size() || y < 0 || y >= houseMap[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }

    int newDirtLevel = std::max(houseMap[x][y] - decreaseBy, 0); // In case decreaseBy given is too big
    totalDirt = totalDirt - (houseMap[x][y] - newDirtLevel);
    houseMap[x][y] = newDirtLevel;
}

bool House::isWall(int x, int y) const {
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

void House::printHouse() const {
    std::cout << "House:" << std::endl;
    for (const auto &row: houseMap) {
        for (const auto &cell: row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}
