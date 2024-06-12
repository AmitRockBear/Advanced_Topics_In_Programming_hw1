#include <vector>
#include <sstream>

#ifndef FileDataExtractor_H
#define FileDataExtractor_H

class FileDataExtractor {
public:
    FileDataExtractor(const std::string& filename);

    int getDockingX() const;
    int getDockingY() const;
    int getMaxSteps() const;
    int getMaxBatterySteps() const;
    const std::vector<std::vector<int>>& getHouseMap() const;

private:
    int dockingX;
    int dockingY;
    int maxSteps;
    int maxBatterySteps;
    std::vector<std::vector<int>> houseMap;

    void readAndExtract(const std::string& filename);
    void readAndExtractHouseData(std::ifstream& file);
    void readAndExtractMaxSteps(std::ifstream& file);
    void readAndExtractMaxBatterySteps(std::ifstream& file);
};

#endif