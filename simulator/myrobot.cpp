#include "MySimulator.h"
#include "../common/AlgorithmRegistrar.h"
#include "../common/Logger.h"
#include "../common/Utils.h"
#include <iostream>
#include <dlfcn.h>

int main(int argc, char *argv[]) {
    void * deterministic_handle = nullptr;
    void * random_handle = nullptr;
    try {
        if (argc != 2) {
            throw std::runtime_error("The program should get only 1 argument, the file name");
        }
        MySimulator simulator;

        const std::string& fileName = argv[1];

        Logger& logger = Logger::getInstance();
        logger.logInfo("Starting the program with file: " + fileName);
        simulator.readHouseFile(fileName);

        deterministic_handle = dlopen("./lib206507923_322853813_DeterministicAlgorithm.so", RTLD_LAZY | RTLD_GLOBAL);
        if (deterministic_handle == nullptr)
        {
            throw std::runtime_error(dlerror());
        }
        random_handle = dlopen("./lib206507923_322853813_RandomAlgorithm.so", RTLD_LAZY | RTLD_GLOBAL);
        if (random_handle == nullptr)
        {
            dlclose(deterministic_handle);
            throw std::runtime_error(dlerror());
        }

        auto registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
        for(const auto& algo: registrar) {
            auto algorithm = algo.create();
            simulator.setAlgorithm(std::move(algorithm));
            // break; // to choose algorithm TODO delete later
        }

        simulator.run();
        logger.logInfo("Program finished successfully");
    } catch (const std::exception& e) {
        handleException(e);
        return 1;
    } catch (...) {
        handleException(std::runtime_error("An unknown unrecoverable error occurred."));
        return 1;
    }
    return 0;

}