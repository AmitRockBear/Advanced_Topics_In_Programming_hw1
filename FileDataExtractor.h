#ifndef FileDataExtractor_H
#define FileDataExtractor_H

#include <vector>
#include <fstream>
#include <string>

class FileDataExtractor {
public:
    FileDataExtractor();

    int getDockingX() const;
    int getDockingY() const;
    int getMaxSteps() const;
    int getMaxBatterySteps() const;
    std::vector<std::vector<int>>& getHouseMap();
    bool readAndExtract(const std::string& fileName);

private:
    int dockingX;
    int dockingY;
    int maxSteps;
    int maxBatterySteps;
    std::vector<std::vector<int>> houseMap;

    void readAndExtractHouseData(std::ifstream& file);
    void readAndExtractMaxSteps(std::ifstream& file);
    void readAndExtractMaxBatterySteps(std::ifstream& file);
    bool isNextCharacterSpaceOrEndOfLine(std::istringstream& iss);
};

#endif