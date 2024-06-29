#include "FileDataExtractor.h"
#include "Logger.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

FileDataExtractor::FileDataExtractor() {
    Logger::getInstance().logInfo("Initializing FileDataExtractor");
}

int FileDataExtractor::getDockingX() const {
    return dockingX;
}

int FileDataExtractor::getDockingY() const {
    return dockingY;
}

int FileDataExtractor::getMaxSteps() const {
    return maxSteps;
}

int FileDataExtractor::getMaxBatterySteps() const {
    return maxBatterySteps;
}

std::vector<std::vector<int>>& FileDataExtractor::getHouseMap() {
    return houseMap;
}

bool FileDataExtractor::readAndExtract(const std::string& fileName) {
    Logger::getInstance().logInfo("Starting to read and extract data from file: " + fileName);

    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open input file: " + fileName);
    }

    readAndExtractMaxBatterySteps(file);
    readAndExtractMaxSteps(file);
    readAndExtractHouseData(file);

    file.close();
    return true;
}

void FileDataExtractor::readAndExtractMaxBatterySteps(std::ifstream& file) {
    Logger::getInstance().logInfo("Reading maxBatterySteps from input file");

    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error reading maxBatterySteps from input file");
    }

    std::istringstream iss(line);
    if (!(iss >> maxBatterySteps)) {
        throw std::runtime_error("Invalid format for maxBatterySteps in input file");
    }

    if (!isNextCharacterSpaceOrEndOfLine(iss)) {
        throw std::runtime_error("Invalid format: maxBatterySteps must be followed by a space or EOF in input file");
    }

    Logger::getInstance().logInfo("Successfully read maxBatterySteps from input file: " + std::to_string(maxBatterySteps));
}

void FileDataExtractor::readAndExtractMaxSteps(std::ifstream& file) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Reading maxSteps from input file");

    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error reading maxSteps from input file");
    }

    std::istringstream iss(line);
    if (!(iss >> maxSteps)) {
        throw std::runtime_error("Invalid format for maxSteps in input file");
    }

    if (!isNextCharacterSpaceOrEndOfLine(iss)) {
        throw std::runtime_error("Invalid format: maxSteps must be followed by a space or EOF in input file");
    }

    logger.logInfo("Successfully read maxSteps from input file: " + std::to_string(maxSteps));
}

void FileDataExtractor::readAndExtractHouseData(std::ifstream& file) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Reading house data from input file");

    std::string line;
    int row = 0;
    int dockingStationCount = 0;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] == ' ') {
            throw std::runtime_error("Invalid format: house map must not start with a space character in input file");
        }

        std::vector<int> row_data;
        std::istringstream iss(line);
        char ch;
        while (iss >> ch) {
            if (ch >= '0' && ch <= '9') {
                row_data.push_back(ch - '0');
            } else if (ch == 'W' || ch == '-') {
                row_data.push_back(-1);
            } else if (ch == 'D') {
                if (dockingStationCount > 0) {
                    throw std::runtime_error("Invalid house map: can only have one docking station in input file");
                }
                dockingX = row;
                dockingY = row_data.size();
                dockingStationCount++;
                row_data.push_back(0);
            } else {
                throw std::runtime_error("Invalid house map: invalid character found: '" + std::string(1, ch) + "', in input file");
            }

            if (!isNextCharacterSpaceOrEndOfLine(iss)) {
                throw std::runtime_error("Invalid format: every character in house map must be followed by a space or EOF in input file");
            }
        }
        houseMap.push_back(row_data);
        row++;
    }

    if (dockingStationCount == 0) {
        throw std::runtime_error("Invalid house map: no docking station found in input file");
    }

    logger.logInfo("Successfully read house data from input file");
}
