#include "Controller.h"
#include "FileDataExtractor.h"
#include <iostream>
#include <fstream>
#include <sstream>

void handleException(const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            throw std::runtime_error("The program should get only 1 argument, the file name");
        }

        const std::string& fileName = argv[1];
        FileDataExtractor inputData = FileDataExtractor();
        inputData.readAndExtract(fileName);
        
        int dockingX = inputData.getDockingX();
        int dockingY = inputData.getDockingY();
        House house(inputData.getHouseMap(), dockingX, dockingY);
        VacuumCleaner vacuumCleaner(dockingX, dockingY, inputData.getMaxBatterySteps());
        Controller controller(house, vacuumCleaner, inputData.getMaxSteps());
        controller.run();
    } catch (const std::exception& e) {
        handleException(e);
        return 1;
    } catch (...) {
        handleException(std::runtime_error("An unknown unrecoverable error occurred."));
        return 1;
    }

    return 0;

}