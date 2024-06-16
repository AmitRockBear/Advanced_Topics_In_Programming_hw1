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
    double getBatteryLevel() const;
    int getMaxBatterySteps() const;
    bool isAtLocation(int locationX, int locationY) const;
    int getX() const;
    int getY() const;
    void setX(int newX);
    void setY(int newY);

private:
    Point location;
    double battery;
    int maxBatterySteps;
};

#endif
