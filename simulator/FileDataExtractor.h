#ifndef FileDataExtractor_H
#define FileDataExtractor_H

#include <vector>
#include <fstream>
#include <string>

class FileDataExtractor {
public:
    FileDataExtractor();
    ssize_t getDockingX() const;
    ssize_t getDockingY() const;
    std::size_t getMaxSteps() const;
    std::size_t getMaxBattery() const;
    std::vector<std::vector<int>>& getHouseMap();
    bool readAndExtract(const std::string& fileName);

    FileDataExtractor(const FileDataExtractor&) = delete;
    FileDataExtractor& operator=(const FileDataExtractor&) = delete;
    FileDataExtractor(FileDataExtractor&&) = delete;
    FileDataExtractor& operator=(FileDataExtractor&&) = delete;
private:
    ssize_t dockingX;
    ssize_t dockingY;
    std::size_t maxSteps;
    std::size_t maxBattery;
    std::vector<std::vector<int>> houseMap;

    void readHouseName(std::ifstream& file);
    void readAndExtractHouseData(std::ifstream& file);
    void readAndExtractKeys(std::ifstream& file, const std::vector<std::string>& keys, std::vector<std::string>& values);
};

#endif