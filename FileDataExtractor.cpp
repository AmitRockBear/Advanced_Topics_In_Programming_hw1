#include <iostream>
#include <fstream>
#include <sstream>
#include "FileDataExtractor.h"

FileDataExtractor::FileDataExtractor() {
};

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

const std::vector<std::vector<int>>& FileDataExtractor::getHouseMap() const {
    return houseMap;
}

bool FileDataExtractor::readAndExtract(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return false;
    }

    try {
        readAndExtractMaxBatterySteps(file);
        readAndExtractMaxSteps(file);
        readAndExtractHouseData(file);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    file.close();
    return true;
}

void FileDataExtractor::readAndExtractMaxBatterySteps(std::ifstream& file) {
    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error reading maxBatterySteps");
    }

    std::istringstream iss(line);
    if (!(iss >> maxBatterySteps)) {
        throw std::runtime_error("Invalid format for maxBatterySteps in house file");
    }
}

void FileDataExtractor::readAndExtractMaxSteps(std::ifstream& file) {
    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Error reading maxSteps");
    }

    std::istringstream iss(line);
    if (!(iss >> maxSteps)) {
        throw std::runtime_error("Invalid format for maxSteps in house file");
    }
}

void FileDataExtractor::readAndExtractHouseData(std::ifstream& file) {
    std::string line;
    int row = 0;
    while (std::getline(file, line)) {
        std::vector<int> row_data;
        std::istringstream iss(line);
        char ch;
        while (iss >> ch) {
            if (ch >= '0' && ch <= '9') {
                row_data.push_back(ch - '0');
            } else if (ch == 'W') {
                row_data.push_back(-1);
            } else if (ch == 'D') {
                dockingX = row;
                dockingY = row_data.size();
                row_data.push_back(0); // No dirt at docking station
            } else {
                throw std::runtime_error("Invalid character in house file: " + std::string(1, ch));
            }
        }
        houseMap.push_back(row_data);
        row++;
    }
}
