#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include "Point.h"

static const char *const HORIZONTAL_WALL = "_";

static const char *const VERTICAL_WALL = "|";

static const char *const VACUUM_SIGN = "V";

static const char *const DOCKING_SIGN = "D";

static const int WAITING_TIME = 200;

class House {
public:
    House(const std::vector<std::vector<int>>& houseMap, int dockingX, int dockingY);

    int getDirtLevel(Point& location) const;
    void decreaseDirtLevel(Point& location, int decreaseBy);
    bool isWall(Point& location) const;
    int getTotalDirt() const;
    void getDockingLocation(Point& locationToModify) const;
    void houseVisualization(Point vacuumLocation) const;

private:
    size_t getMaxY(std::vector<std::vector<int>> &houseMap);
    int calcTotalDirt() const;
    
    int totalDirt;
    std::vector<std::vector<int>> houseMap;
    Point dockingLocation;
};

#endif
