#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include <string>
#include <stdexcept>
#include "point.h"

class House {
public:
    House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY);

    int getDirtLevel(int x, int y) const;
    void decreaseDirtLevel(int x, int y, int decreaseBy);
    bool isWall(int x, int y) const;
    int getDockingX() const;
    int getDockingY() const;
    int getTotalDirt() const;
    void printHouse() const;

private:
    int totalDirt;
    int calcTotalDirt() const;
    std::vector<std::vector<int>> houseMap;
    Point dockingLocation;
};

#endif
