#include "DeterministicAlgorithm.h"
#include "AlgorithmRegistration.h"

DeterministicAlgorithm::DeterministicAlgorithm() : Algorithm::Algorithm() {
}

REGISTER_ALGORITHM(DeterministicAlgorithm);

Step DeterministicAlgorithm::ChooseStep(std::vector<Step> &moves) {
    return moves[0];
}