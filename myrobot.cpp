#include "MySimulator.h"
#include "Algorithm.h"
#include "Logger.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

        Algorithm algorithm;
        simulator.setAlgorithm(algorithm);
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