#ifndef POINT_H
#define POINT_H
#include "General.h"
#include "Direction.h"
#include <string>

class Point {
private:
    ssize_t x;
    ssize_t y;
public:
    Point(ssize_t x=0, ssize_t y=0);
    ssize_t getX() const;
    void setX(ssize_t newX);
    ssize_t getY() const;
    void setY(ssize_t newY);
    void moveToNeighbor(Direction direction);
    void move(Step direction);
    bool operator==(const Point& other) const;
    std::string toString() const;
    bool operator<(const Point& other) const;
    Point getNeighbor(Direction direction) const;
    Step getStepToGetToNeighborPoint(Point p) const;
    
    // Point() = delete;
    // Point(const Point&) = delete;
    // Point& operator=(const Point&) = delete;
    // Point(Point&&) = delete;
    // Point& operator=(Point&&) = delete;
};


// Hash function for Point
template <>
struct std::hash<Point> {
    std::size_t operator()(const Point &p) const {
        return std::hash<ssize_t>()(p.getX()) ^ std::hash<ssize_t>()(p.getY());
    }
};


#endif
