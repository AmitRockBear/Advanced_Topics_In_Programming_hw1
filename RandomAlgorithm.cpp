#include "RandomAlgorithm.h"
#include <ctime>
#include "AlgorithmRegistration.h"

RandomAlgorithm::RandomAlgorithm() : Algorithm::Algorithm() {
    std::srand(std::time(nullptr));
}

REGISTER_ALGORITHM(RandomAlgorithm);

Step RandomAlgorithm::ChooseStep(std::vector<Step> &moves) {
    return moves[std::rand() % moves.size()];
}