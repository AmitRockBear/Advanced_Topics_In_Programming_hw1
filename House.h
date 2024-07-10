#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include <memory>
#include "Point.h"

static const char *const HORIZONTAL_WALL = "_";

static const char *const VERTICAL_WALL = "|";

static const char *const VACUUM_SIGN = "V";

static const char *const DOCKING_SIGN = "D";

static const int WAITING_TIME = 200;

class House {
public:
    House();
    int getDirtLevel(Point& location) const;
    void decreaseDirtLevel(Point& location, int decreaseBy);
    bool isWall(Point& location) const;
    int getTotalDirt() const;
    void getDockingLocation(Point& locationToModify) const;
    void houseVisualization(Point vacuumLocation) const;
    void setDockingLocation(std::size_t x, std::size_t y);
    void setHouseMap(std::vector<std::vector<int>>& houseMap);

private:
    int calcTotalDirt() const;
    
    int totalDirt;
    std::unique_ptr<std::vector<std::vector<int>>> houseMap;
    Point dockingLocation;
};

#endif
