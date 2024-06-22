#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <functional>
#include "Point.h"
#include <vector>
#include <stack>

class Algorithm {
public:
    Algorithm(std::function<double()> batterySensor, std::function<bool(char)> wallSensor, std::function<int(char)> dirtSensor);
    char decideNextStep();
private:
    std::function<double()> batterySensor;
    std::function<bool(char)> wallSensor;
    std::function<int(char)> dirtSensor;
    static const std::vector<char> directions;
    std::stack<char> stepsBack;
    void calcValidMoves(std::vector<char>& moves);
    Point distanceFromDock;
    bool isBacktracking;
};

#endif // ALGORITHM_H
