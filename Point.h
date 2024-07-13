#ifndef POINT_H
#define POINT_H
#include "General.h"
#include "Direction.h"
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
    void move(Step direction);
    bool operator==(const Point& other) const;
    std::string toString() const;
    bool operator<(const Point& other) const;
    Point getNeighbor(Direction direction) const;
    Step getStepToGetToNeighborPoint(Point p) const;
};


// Hash function for Point
template <>
struct std::hash<Point> {
    std::size_t operator()(const Point &p) const {
        return std::hash<size_t>()(p.getX()) ^ std::hash<size_t>()(p.getY());
    }
};


#endif
