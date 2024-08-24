#ifndef ADVANCED_TOPICS_IN_PROGRAMMING_HW1_ALGORITHMREGISTRAR_H
#define ADVANCED_TOPICS_IN_PROGRAMMING_HW1_ALGORITHMREGISTRAR_H

#include <functional>
#include <memory>
#include "abstract_algorithm.h"

using AlgorithmFactory = std::function<std::unique_ptr<AbstractAlgorithm>()>;

class AlgorithmRegistrar {
    class AlgorithmFactoryPair {
        std::string name_;
        AlgorithmFactory algorithmFactory_;
    public:
        AlgorithmFactoryPair(const std::string& name, AlgorithmFactory algorithmFactory)
                : name_(name), algorithmFactory_(std::move(algorithmFactory)) {}
        // NOTE: API is guaranteed, actual implementation may change
        const std::string& name() const { return name_; }
        std::unique_ptr<AbstractAlgorithm> create() const { return algorithmFactory_(); }
    };
    std::vector<AlgorithmFactoryPair> algorithms;
    static AlgorithmRegistrar registrar;
public:
    // NOTE: API is guaranteed, actual implementation may change
    static AlgorithmRegistrar& getAlgorithmRegistrar();
    void registerAlgorithm(const std::string& name, AlgorithmFactory algorithmFactory) {
        printf("ADDED ALGORITHM %s\n--------------------------\n", name.c_str());
        algorithms.emplace_back(name, std::move(algorithmFactory));
        printf("algorithms amount: %zu\n--------------------------\n", algorithms.size());

    }
    auto begin() const { return algorithms.begin(); }
    auto end() const { return algorithms.end(); }
    std::size_t count() const { return algorithms.size(); }
    void clear() { algorithms.clear(); }
};


#endif