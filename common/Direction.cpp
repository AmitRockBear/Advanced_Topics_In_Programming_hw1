#include "direction.h"

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
        case Step::North: return "N";
        case Step::East: return "E";
        case Step::South: return "S";
        case Step::West: return "W";
        case Step::Stay: return "s";
        case Step::Finish: return "F";
    }
    return "Unknown Step";
}

Step oppositeStep(Step move) {
    switch (move) {
        case Step::North: return Step::South;
        case Step::East: return Step::West;
        case Step::South: return Step::North;
        case Step::West: return Step::East;
        default: return Step::Stay; // In this case, the algorithm will make sure this won't be added to the backtrack path
    }
}