#ifndef ADVANCED_TOPICS_IN_PROGRAMMING_HW1_RANDOMALGORITHM_H
#define ADVANCED_TOPICS_IN_PROGRAMMING_HW1_RANDOMALGORITHM_H
#include "Algorithm.h"

class RandomAlgorithm : public Algorithm {
public:
    RandomAlgorithm();
private:
    virtual Step ChooseStep(std::vector<Step> &moves) override;
};

#endif
