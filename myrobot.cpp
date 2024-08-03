#include "MySimulator.h"
#include "AlgorithmRegistrar.h"
#include "Logger.h"
#include "Utils.h"
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            throw std::runtime_error("The program should get only 1 argument, the file name");
        }
        MySimulator simulator;

        const std::string& fileName = argv[1];

        Logger& logger = Logger::getInstance();
        logger.logInfo("Starting the program with file: " + fileName);
        simulator.readHouseFile(fileName);

        //auto handle = dlopen("RandomAlgorithm.so", RTLD_LAZY);
        auto registrar = AlgorithmRegistrar::getAlgorithmRegistrar();
        for(const auto& algo: registrar) {
            auto algorithm = algo.create();
            simulator.setAlgorithm(std::move(algorithm));
            break;
        }

        simulator.run();
        //dlclose(handle);

        //Algorithm algorithm;
        //RandomAlgorithm myalg;
        //Algorithm myalg = RandomAlgorithm();
        //simulator.setAlgorithm(static_cast<Algorithm>(myalg));
        //simulator.run();
//        AlgorithmRegistrar::getAlgorithmRegistrar().clear();
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