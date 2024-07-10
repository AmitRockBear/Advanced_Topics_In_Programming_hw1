#ifndef DIRECTION_H
#define DIRECTION_H

#include <iostream>

enum class Direction { North, East, South, West };
enum class Step { North, East, South, West, Stay, Finish };

Step DirectionToStep(Direction direction);
Direction MovableStepToDirection(Step step);
std::string toString(Direction d);
std::string toString(Step s);
Step oppositeStep(Step move);

#endif