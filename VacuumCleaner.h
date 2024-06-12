#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H

#include "Point.h"
#include <stack>

class VacuumCleaner {
public:
    VacuumCleaner(int x, int y, int maxBatterySteps);

    void move(char direction);
    void increaseChargeBy(int steps);
    void decreaseChargeBy(int steps);
    int getBatteryLevel() const;
    int getMaxBatterySteps() const;
    bool isAtLocation(int locationX, int locationY) const;
    int getPathSize() const;
    int getX() const;
    int getY() const;
    bool getIsBackTracking() const;
    std::stack<char>& getAllSteps();
    void setX(int newX);
    void setY(int newY);
    void setMaxBatterySteps(int newMaxBatterySteps);
    void flipIsBackTrackingStatus();
    char backtrack();

private:
    Point location;
    int battery;
    int maxBatterySteps;
    bool isBackTracking;
    std::stack<char> path;
    std::stack<char> allSteps;
};

#endif // VACUUMCLEANER_H
