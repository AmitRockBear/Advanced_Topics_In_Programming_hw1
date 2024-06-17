#ifndef POINT_H
#define POINT_H
#include "General.h"

class Point {
private:
    int x;
    int y;
public:
    Point();
    Point(int x, int y);
    int getX() const;
    void setX(int newX);
    int getY() const;
    void setY(int newY);
    Point getNeighbor(char direction=STAY);
    void move(char direction);
};

#endif // POINT_H
