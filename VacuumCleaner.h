#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H
#include "Point.h"

class VacuumCleaner {
public:
    VacuumCleaner(ssize_t x = 0, ssize_t y = 0, size_t maxBatterySteps = 0);

    void move(Step direction);
    void increaseChargeBy(std::size_t steps);
    void decreaseChargeBy(std::size_t steps);
    double getBatteryLevel() const;
    std::size_t getMaxBatterySteps() const;
    bool isAtLocation(Point& otherLocation) const;
    ssize_t getX() const;
    ssize_t getY() const;
    void getLocation(Point& locationToModify) const;

private:
    Point location;
    double battery;
    std::size_t maxBatterySteps;
};

#endif
