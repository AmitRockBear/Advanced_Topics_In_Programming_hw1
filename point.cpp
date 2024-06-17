#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}

Point::Point() : x(0), y(0) {}


int Point::getX() const {
    return x;
}

void Point::setX(int newX) {
    x = newX;
}

int Point::getY() const {
    return y;
}

void Point::setY(int newY) {
    y = newY;
}

void Point::move(char direction) {
    switch (direction) {
        case 'N': setX(x-1); break;
        case 'E': setY(y+1); break;
        case 'S': setX(x+1); break;
        case 'W': setY(y-1); break;
        default: break;
    }
}

Point Point::getNeighbor(char direction) const {
    Point neighbor = Point(getX(), getY());
    neighbor.move(direction);
    return neighbor;
}