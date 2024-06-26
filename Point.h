#ifndef POINT_H
#define POINT_H
#include "General.h"
#include <string>

class Point {
private:
    int x;
    int y;
public:
    Point(int x=0, int y=0);
    int getX() const;
    void setX(int newX);
    int getY() const;
    void setY(int newY);
    void moveToNeighbor(char direction=STAY);
    void move(char direction);
    bool operator==(const Point& other) const;
    std::string toString() const;
};

#endif
