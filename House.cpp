#include "House.h"
#include <fstream>
#include <sstream>
#include <iostream>

House::House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY)
    : houseMap(houseMap), dockingLocation(Point(dockingX, dockingY)), totalDirt(getTotalDirt()) {}

int House::getTotalDirt() const {
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
    houseMap[x][y] = houseMap[x][y] - decreaseBy;
    totalDirt = totalDirt - decreaseBy;
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
