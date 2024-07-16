#ifndef DIRECTION_H
#define DIRECTION_H

#include "enums.h"
#include <iostream>

Step DirectionToStep(Direction direction);
Direction MovableStepToDirection(Step step);
std::string toString(Direction d);
std::string toString(Step s);
Step oppositeStep(Step move);

#endif