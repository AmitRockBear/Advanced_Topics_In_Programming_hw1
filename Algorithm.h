#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "VacuumCleaner.h"
#include <vector>
#include <functional>
#include "Point.h"

class Algorithm {
public:
    Algorithm(std::function<double()> battery_sensor, std::function<bool(char)> wall_sensor, std::function<int(char)> dirt_sensor);


    char decideNextStep();


private:
    std::function<double()> battery_sensor;
    std::function<bool(char)> wall_sensor;
    std::function<int(char)> dirt_sensor;
    static const std::vector<char> directions;
    std::stack<char> stepsBack;
    std::vector<char> calcValidMoves();
    Point distanceFromDock;
    bool isBacktracking;
};

#endif // ALGORITHM_H
