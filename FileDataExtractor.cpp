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

int FileDataExtractor::getMaxBattery() const {
    return maxBattery;
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
    
    try {
        readHouseName(file);
        const std::vector<std::string> keys = {"MaxSteps", "MaxBattery", "Rows", "Cols"}; 
        std::vector<std::string> values(keys.size());
        readAndExtractKeys(file, keys, values);
        maxSteps = std::stoi(values[0]);
        maxBattery = std::stoi(values[1]);
        int rows = std::stoi(values[2]);
        int cols = std::stoi(values[3]);
        if (rows < 0 || cols < 0) {
            throw std::runtime_error("Invalid house dimensions in input file");
        }
        houseMap = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
        readAndExtractHouseData(file);
    } catch (...) {
        file.close();
        throw;
    }

    file.close();
    return true;
}

void FileDataExtractor::readHouseName(std::ifstream& file) {
    Logger& logger = Logger::getInstance();
    std::string line, houseName;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error reading house name from input file");
    }
    houseName = trim(line);

    logger.logInfo("Welcome to house: " + houseName + "!");
}

void FileDataExtractor::readAndExtractKeys(std::ifstream& file, const std::vector<std::string>& keys, std::vector<std::string>& values) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Reading keys from input file");

    std::string line;
    for (size_t i = 0; i < keys.size(); i++) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("Error reading keys from input file");
        }

        std::string key = keys[i];
        std::string value;
        bool extractedSuccessfully = extractKeyValue(line, key, value);
        if (!extractedSuccessfully) {
            throw std::runtime_error("Invalid format for keys in input file");
        }
        if (key != keys[i]) {
            throw std::runtime_error("Invalid format: " + keys[i] + " key not found where expected in input file");
        }

        values[i] = value;
    }

    logger.logInfo("Successfully read keys from input file");
}

void FileDataExtractor::readAndExtractHouseData(std::ifstream& file) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Reading house data from input file");

    std::string line;
    int dockingStationCount = 0;
    for (size_t row = 0; row < houseMap.size(); row++) {
        if (!std::getline(file, line))
            continue;
        
        std::istringstream iss(line);
        char ch;
        for (size_t col = 0; col < houseMap[row].size(); col++) {
            if (!(iss.get(ch))) break;
            if (ch >= '0' && ch <= '9') {
                houseMap[row][col] = ch - '0';
            } else if (ch == 'W') {
                houseMap[row][col] = -1;
            } else if (ch == 'D') {
                if (dockingStationCount > 0) {
                    throw std::runtime_error("Invalid house map: can only have one docking station in input file");
                }
                dockingX = row;
                dockingY = col;
                dockingStationCount++;
                houseMap[row][col] = 0;
            }
        }
    }

    if (dockingStationCount == 0) {
        throw std::runtime_error("Invalid house map: no docking station found in input file");
    }

    logger.logInfo("Successfully read house data from input file");
}
