#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H
#include "Point.h"

class VacuumCleaner {
public:
    VacuumCleaner(std::size_t x, std::size_t y, std::size_t maxBatterySteps);

    void move(Direction direction);
    void increaseChargeBy(std::size_t steps);
    void decreaseChargeBy(std::size_t steps);
    double getBatteryLevel() const;
    std::size_t getMaxBatterySteps() const;
    bool isAtLocation(Point& otherLocation) const;
    std::size_t getX() const;
    std::size_t getY() const;
    void getLocation(Point& locationToModify) const;

private:
    Point location;
    double battery;
    std::size_t maxBatterySteps;
};

#endif
