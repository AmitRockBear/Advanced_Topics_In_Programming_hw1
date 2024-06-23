#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include "Point.h"

class House {
public:
    House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY);

    int getDirtLevel(Point& location) const;
    void decreaseDirtLevel(Point& location, int decreaseBy);
    bool isWall(Point& location) const;
    int getTotalDirt() const;
    void getDockingLocation(Point& locationToModify) const;
    void houseVisualization(Point vacuumLocation) const;
    void printHouse() const;

private:
    void padHouseMap(std::vector<std::vector<int>>& houseMap);
    size_t getMaxY(std::vector<std::vector<int>> &houseMap);
    int totalDirt;
    int calcTotalDirt() const;
    std::vector<std::vector<int>> houseMap;
    Point dockingLocation;
};

#endif
