#ifndef POINT_H
#define POINT_H
#include "General.h"
#include "skeleton.h"
#include <string>

class Point {
private:
    std::size_t x;
    std::size_t y;
public:
    Point(std::size_t x=0, std::size_t y=0);
    std::size_t getX() const;
    void setX(std::size_t newX);
    std::size_t getY() const;
    void setY(std::size_t newY);
    void moveToNeighbor(Direction direction);
    void move(Direction direction);
    bool operator==(const Point& other) const;
    std::string toString() const;
};

#endif
