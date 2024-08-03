#ifndef ADVANCED_TOPICS_IN_PROGRAMMING_HW1_ALGORITHMREGISTRATION_H
#define ADVANCED_TOPICS_IN_PROGRAMMING_HW1_ALGORITHMREGISTRATION_H
#include "AlgorithmRegistrar.h"
struct AlgorithmRegistration {
    AlgorithmRegistration(const std::string& name, AlgorithmFactory algorithmFactory) {
        AlgorithmRegistrar::getAlgorithmRegistrar()
                .registerAlgorithm(name, std::move(algorithmFactory));
    }
};

#define REGISTER_ALGORITHM(ALGO) AlgorithmRegistration \
   _##ALGO(#ALGO, []{return std::make_unique<ALGO>();})

#endif
