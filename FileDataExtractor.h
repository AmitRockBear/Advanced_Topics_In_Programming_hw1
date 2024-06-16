#include <vector>
#include <sstream>

#ifndef FileDataExtractor_H
#define FileDataExtractor_H

class FileDataExtractor {
public:
    FileDataExtractor();

    int getDockingX() const;
    int getDockingY() const;
    int getMaxSteps() const;
    int getMaxBatterySteps() const;
    const std::vector<std::vector<int>>& getHouseMap() const;

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
};

#endif