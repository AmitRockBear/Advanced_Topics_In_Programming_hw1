#include "Point.h"
#include "skeleton.h"
#include <sstream>

Point::Point(std::size_t x, std::size_t y) : x(x), y(y) {}

std::size_t Point::getX() const {
    return x;
}

void Point::setX(std::size_t newX) {
    x = newX;
}

std::size_t Point::getY() const {
    return y;
}

void Point::setY(std::size_t newY) {
    y = newY;
}

void Point::move(Direction direction) {
    switch (direction) {
        case Direction::North: setX(x-1); break;
        case Direction::East: setY(y+1); break;
        case Direction::South: setX(x+1); break;
        case Direction::West: setY(y-1); break;
        default: break;
    }
}

void Point::moveToNeighbor(Direction direction) {
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