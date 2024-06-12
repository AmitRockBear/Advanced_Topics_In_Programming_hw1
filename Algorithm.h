#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "VacuumCleaner.h"
#include <vector>

class Algorithm {
public:
    Algorithm();

    char decideNextStep();

private:
    std::vector<char> getValidMoves() const;
};

#endif // ALGORITHM_H
