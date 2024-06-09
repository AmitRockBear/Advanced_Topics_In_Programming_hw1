#include "Algorithm.h"
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time>
#include <algorithm> // For std::random_shuffle

Algorithm::Algorithm(VacuumCleaner* vacuumCleaner) : vacuumCleaner(vacuumCleaner) {
    std::srand(std::time(nullptr)); // Seed random number generator
}

char Algorithm::decideNextStep() {
    try {
        std::vector<char> validMoves = getValidMoves();
        if (validMoves.empty()) {
            return 'S'; // Stay in place if no valid moves
        }
        return validMoves[std::rand() % validMoves.size()];
    } catch (const std::exception& e) {
        std::cerr << "Error deciding next step: " << e.what() << std::endl;
        return 'S'; // Default to staying in place on error
    }
}

std::vector<char> Algorithm::getValidMoves() const {
    std::vector<char> validMoves;
    for (char direction : directions) {
        if (!vacuumCleaner->isWall(direction)) {
            validMoves.push_back(direction);
        }
    }
    return validMoves;
}
