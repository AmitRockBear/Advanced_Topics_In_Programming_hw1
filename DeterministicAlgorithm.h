#ifndef ADVANCED_TOPICS_IN_PROGRAMMING_HW1_DETERMINISTICALGORITHM_H
#define ADVANCED_TOPICS_IN_PROGRAMMING_HW1_DETERMINISTICALGORITHM_H
#include "Algorithm.h"

class DeterministicAlgorithm : public Algorithm {
public:
    DeterministicAlgorithm();
private:
    virtual Step ChooseStep(std::vector<Step> &moves) override;
};

#endif //ADVANCED_TOPICS_IN_PROGRAMMING_HW1_DETERMINISTICALGORITHM_H
