#include "Point.h"
#include <sstream>

Point::Point(ssize_t x, ssize_t y) : x(x), y(y) {}

ssize_t Point::getX() const {
    return x;
}

void Point::setX(ssize_t newX) {
    x = newX;
}

ssize_t Point::getY() const {
    return y;
}

void Point::setY(ssize_t newY) {
    y = newY;
}

void Point::move(Step direction) {
    switch (direction) {
        case Step::North: setX(x-1); break;
        case Step::East: setY(y+1); break;
        case Step::South: setX(x+1); break;
        case Step::West: setY(y-1); break;
        default: break;
    }
}

Step Point::getStepToGetToNeighborPoint(Point p) const {
    if (x < p.x)
        return Step::South;
    if (x > p.x)
        return Step::North;
    if (y < p.y)
        return Step::East;
    if (y > p.y)
        return Step::West;
    return Step::Stay;
}

void Point::moveToNeighbor(Direction direction) {
    (*this).move(DirectionToStep(direction));
}

Point Point::getNeighbor(Direction direction) const {
    Point neighbor(getX(), getY());
    neighbor.move(DirectionToStep(direction));
    return neighbor;
}

bool Point::operator==(const Point& other) const {
    return (this->x == other.x) && (this->y == other.y);
}

bool Point::operator<(const Point& other) const {
    if (this->x < other.x) return true;
    if (this->x == other.x && this->y < other.y) return true;
    return false;
}

std::string Point::toString() const {
    std::stringstream stringStream;
    stringStream << "Point(" << x << ", " << y << ")";
    return stringStream.str();
}

