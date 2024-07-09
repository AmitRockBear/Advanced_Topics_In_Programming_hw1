#ifndef SKELETON_H
#define SKELETON_H

#include <iostream>

// [1]
enum class Direction { North, East, South, West };
enum class Step { North, East, South, West, Stay, Finish };

Step DirectionToStep(Direction direction) {
    switch (direction) {
        case Direction::North: return Step::North;
        case Direction::East: return Step::East;
        case Direction::South: return Step::South;
        default: return Step::West;
    }
}
Direction MovableStepToDirection(Step step) {
    switch (step) {
        case Step::North: return Direction::North;
        case Step::East: return Direction::East;
        case Step::South: return Direction::South;
        case Step::West: return Direction::West;
        default: return Direction::West;
    }
}
std::string toString(Direction d) {
    switch(d) {
        case Direction::North: return "North";
        case Direction::East: return "East";
        case Direction::South: return "South";
        case Direction::West: return "West";
    }
    return "Unknown Direction";
}
std::string toString(Step s) {
    switch(s) {
        case Step::North: return "North";
        case Step::East: return "East";
        case Step::South: return "South";
        case Step::West: return "West";
        case Step::Stay: return "Stay";
        case Step::Finish: return "Finish";
    }
    return "Unknown Step";
}

#endif