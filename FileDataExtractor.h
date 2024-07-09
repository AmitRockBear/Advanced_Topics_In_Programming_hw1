#ifndef FileDataExtractor_H
#define FileDataExtractor_H

#include <vector>
#include <fstream>
#include <string>
#include <memory>

class FileDataExtractor {
public:
    FileDataExtractor();
    std::size_t getDockingX() const;
    std::size_t getDockingY() const;
    std::size_t getMaxSteps() const;
    std::size_t getMaxBattery() const;
    std::shared_ptr<std::vector<std::vector<std::size_t>>> getHouseMap();
    bool readAndExtract(const std::string& fileName);

private:
    std::size_t dockingX;
    std::size_t dockingY;
    std::size_t maxSteps;
    std::size_t maxBattery;
    std::shared_ptr<std::vector<std::vector<std::size_t>>> houseMap;

    void readHouseName(std::ifstream& file);
    void readAndExtractHouseData(std::ifstream& file);
    void readAndExtractKeys(std::ifstream& file, const std::vector<std::string>& keys, std::vector<std::string>& values);
};

#endif