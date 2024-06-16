#include "Algorithm.h"
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time>
#include <algorithm> // For std::random_shuffle
#include <iostream>
#include "general.h"

const std::vector<char> Algorithm::directions = {'S', 'W', 'N', 'E'};

Algorithm::Algorithm(std::function<double()> battery_sensor, std::function<bool(char)> wall_sensor,
                     std::function<int(char)> dirt_sensor) {
    this->battery_sensor = battery_sensor;
    this->wall_sensor = wall_sensor;
    this->dirt_sensor = dirt_sensor;
    this->stepsBack = std::stack<char>();
    this->distanceFromDock = {0, 0};
    this->isBacktracking = false;
    std::srand(std::time(nullptr));
}

char oppositeMove(char move) {
    switch (move) {
        case 'N': return 'S';
        case 'E': return 'W';
        case 'S': return 'N';
        case 'W': return 'E';
        default: return STAY; // In this case, the algorithm will make sure this won't be added to the backtrack path
    }
}

std::vector<char> Algorithm::calcValidMoves() {
    std::vector<char> moves = std::vector<char>();
    int stepsAmount = stepsBack.size();

    // If there's no battery, the vacuum can only stay in place
    if (battery_sensor() == 0) {
        isBacktracking = false;
        moves.push_back(STAY);
        return moves;
    }

    // If battery left is the same as the amount of steps to the docking station, the vacuum should go back
    if (battery_sensor() >= stepsAmount && battery_sensor() <= stepsAmount + 1) {
        isBacktracking = true;
        moves.push_back(stepsBack.top());
        stepsBack.pop();
        return moves;
    }

    isBacktracking = false;

    // If there's still dirt, the vacuum will stay and clean
    if(dirt_sensor(STAY) > 0) {
        moves.push_back(STAY);
        return moves;
    }

    // Checking other possible directions
    for (auto &&direction : directions) {
        if(!wall_sensor(direction)) {
            moves.push_back(direction); }
    }
    return moves;
}

char Algorithm::decideNextStep() {
    std::vector<char> validMoves = calcValidMoves();
     try {
         if (validMoves.empty()) {
             return STAY; // Stay in place if no valid moves. C stands for clean
         }
         // Choose the next move randomly
         char nextMove = validMoves[std::rand() % validMoves.size()];
         char oppMove = oppositeMove(nextMove);

         // If the vacuum is moving, and it's not backtracking, we want to save its movement as part of the next backtrack path
         if(oppMove != STAY && (!isBacktracking)) {
             stepsBack.push(oppMove);
             // If we're back at the docking station, we'd like to empty the backtrack path
             auto[x, y] = distanceFromDock;
             auto[newX, newY] = getCoordinate( x, y, nextMove);
             distanceFromDock = {newX, newY};
             if (newX == 0 && newY == 0) {
                 stepsBack = std::stack<char>();
             }
         }

         return nextMove;
     } catch (const std::exception& e) {
         std::cerr << "Error deciding next step: " << e.what() << std::endl;
         return STAY; // Default to staying in place on error
     }
}
