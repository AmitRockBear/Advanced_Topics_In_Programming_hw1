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
    House(std::shared_ptr<std::vector<std::vector<int>>> houseMap, std::size_t dockingX, std::size_t dockingY);

    int getDirtLevel(Point& location) const;
    void decreaseDirtLevel(Point& location, int decreaseBy);
    bool isWall(Point& location) const;
    int getTotalDirt() const;
    void getDockingLocation(Point& locationToModify) const;
    void houseVisualization(Point vacuumLocation) const;

private:
    std::size_t getMaxY();
    int calcTotalDirt() const;
    
    int totalDirt;
    std::shared_ptr<std::vector<std::vector<int>>> houseMap;
    Point dockingLocation;
};

#endif
