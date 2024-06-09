#include "House.h"
#include <fstream>
#include <sstream>
#include <iostream>

House::House(const std::string& filename) {
    try {
        loadHouse(filename);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error loading house: " + std::string(e.what()));
    }
}

void House::loadHouse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

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
                docking_x = row;
                docking_y = row_data.size();
                row_data.push_back(0); // No dirt at docking station
            } else {
                throw std::runtime_error("Invalid character in house file: " + std::string(1, ch));
            }
        }
        house_map.push_back(row_data);
        row++;
    }

    file.close();

    // Set default max_battery_steps and max_steps if not present in the file
    max_battery_steps = 100; // Default value, adjust as needed
    max_steps = 1000;        // Default value, adjust as needed
}

int House::getMaxBatterySteps() const {
    return max_battery_steps;
}

int House::getMaxSteps() const {
    return max_steps;
}

int House::getDirtLevel(int x, int y) const {
    if (x < 0 || x >= house_map.size() || y < 0 || y >= house_map[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }
    return house_map[x][y];
}

void House::setDirtLevel(int x, int y, int level) {
    if (x < 0 || x >= house_map.size() || y < 0 || y >= house_map[0].size()) {
        throw std::out_of_range("Invalid coordinates");
    }
    house_map[x][y] = level;
}

bool House::isWall(int x, int y) const {
    if (x < 0 || x >= house_map.size() || y < 0 || y >= house_map[0].size()) {
        return true;
    }
    return house_map[x][y] == -1;
}

int House::getDockingX() const {
    return docking_x;
}

int House::getDockingY() const {
    return docking_y;
}

int House::getTotalDirt() const {
    int total_dirt = 0;
    for (const auto& row : house_map) {
        for (const auto& cell : row) {
            if (cell > 0) {
                total_dirt += cell;
            }
        }
    }
    return total_dirt;
}
