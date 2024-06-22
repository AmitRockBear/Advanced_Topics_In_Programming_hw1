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
    int getDockingX() const;
    int getDockingY() const;
    int getTotalDirt() const;
    void printHouse() const;
    Point getDockingLocation() const;
    static void padHouseMap(std::vector<std::vector<int>>& houseMap);
  
  private:
    static int getMaxY(std::vector<std::vector<int>> &houseMap);

    int totalDirt;
    int calcTotalDirt() const;
    std::vector<std::vector<int>> houseMap;
    Point dockingLocation;
};

#endif
