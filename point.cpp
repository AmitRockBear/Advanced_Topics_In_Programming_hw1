#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}

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

void Point::moveToNeighbor(char direction) {
    Point neighbor = Point(getX(), getY());
    (*this).move(direction);
    }

bool Point::operator==(const Point& other) const {
    return (this->x == other.x) && (this->y == other.y);
}