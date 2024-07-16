#ifndef GENERAL_H
#define GENERAL_H

#include <string>
#include <vector>
#include "Direction.h"

static const std::string DEFAULT_LOG_FILE_NAME = "application.log";
static const std::string EMPTY_STRING = "";
static const std::vector<Direction> DIRECTIONS = { Direction::South, Direction::West, Direction::North, Direction::East };

#endif
