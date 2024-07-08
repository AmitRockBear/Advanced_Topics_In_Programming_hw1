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
    int getMaxBattery() const;
    std::vector<std::vector<int>>& getHouseMap();
    bool readAndExtract(const std::string& fileName);

private:
    int dockingX;
    int dockingY;
    int maxSteps;
    int maxBattery;
    std::vector<std::vector<int>> houseMap;

    void readHouseName(std::ifstream& file);
    void readAndExtractHouseData(std::ifstream& file);
    void readAndExtractKeys(std::ifstream& file, const std::vector<std::string>& keys, std::vector<std::string>& values);
};

#endif