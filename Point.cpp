#include "Point.h"
#include <sstream>

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
        case NORTH: setX(x-1); break;
        case EAST: setY(y+1); break;
        case SOUTH: setX(x+1); break;
        case WEST: setY(y-1); break;
        default: break;
    }
}

void Point::moveToNeighbor(char direction) {
    (*this).move(direction);
}

bool Point::operator==(const Point& other) const {
    return (this->x == other.x) && (this->y == other.y);
}

std::string Point::toString() const {
    std::stringstream stringStream;
    stringStream << "Point(" << x << ", " << y << ")";
    return stringStream.str();
}