#include "Algorithm.h"
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time>
#include <algorithm> // For std::random_shuffle

Algorithm::Algorithm() {
    std::srand(std::time(nullptr)); // Seed random number generator
}

char Algorithm::decideNextStep(std::vector<char> validMoves) {
    try {
        if (validMoves.empty()) {
            return 'S'; // Stay in place if no valid moves
        }
        return validMoves[std::rand() % validMoves.size()];
    } catch (const std::exception& e) {
        std::cerr << "Error deciding next step: " << e.what() << std::endl;
        return 'S'; // Default to staying in place on error
    }
}
