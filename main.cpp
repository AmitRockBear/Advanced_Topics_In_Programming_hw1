#include "GameMaster.h"
#include "FileDataExtractor.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>\n";
        return 1;
    }

    const std::string& fileName = argv[1];
    FileDataExtractor inputData = FileDataExtractor(fileName);

    int dockingX = inputData.getDockingX();
    int dockingY = inputData.getDockingY();
    House house(inputData.getHouseMap(), dockingX, dockingY);
    VacuumCleaner vacuumCleaner(dockingX, dockingY, inputData.getMaxBatterySteps());
    GameMaster gameMaster(house, vacuumCleaner, inputData.getMaxSteps());
    gameMaster.run();

    return 0;
}