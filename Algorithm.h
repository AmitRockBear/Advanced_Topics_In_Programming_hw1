#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Point.h"
#include <functional>
#include <vector>
#include <stack>

class Algorithm {
public:
    Algorithm(std::function<double()> batterySensor, std::function<bool(char)> wallSensor, std::function<int(char)> dirtSensor);
    char decideNextStep();
private:
    void calcValidMoves(std::vector<char>& moves);
    char oppositeMove(char move) const;
    
    std::function<double()> batterySensor;
    std::function<bool(char)> wallSensor;
    std::function<int(char)> dirtSensor;
    std::stack<char> stepsBack;
    Point distanceFromDock;
    bool isBacktracking;
};

#endif
