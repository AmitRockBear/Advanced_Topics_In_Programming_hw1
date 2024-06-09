#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "VacuumCleaner.h"
#include <vector>

class Algorithm {
public:
    Algorithm(VacuumCleaner* vacuumCleaner);

    char decideNextStep();

private:
    VacuumCleaner* vacuumCleaner;
    std::vector<char> directions {'N', 'E', 'S', 'W'};

    std::vector<char> getValidMoves() const;
};

#endif // ALGORITHM_H
