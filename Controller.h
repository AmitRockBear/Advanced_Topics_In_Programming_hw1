#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "House.h"
#include "VacuumCleaner.h"
#include "Algorithm.h"
#include "General.h"
#include <vector>
#include <string>

class Controller {
public:
    Controller(House& house, VacuumCleaner& vacuumCleaner, int maxSteps, int stepsTaken = 0, bool missionCompleted = false, bool missionFailed = false);
    void run();

private:
    House house;
    VacuumCleaner vacuumCleaner;
    Algorithm algorithm;
    int maxSteps;
    int stepsTaken;
    bool missionCompleted;
    bool missionFailed;
    std::vector<char> steps;
    void vacuumLoop();
    void handleDockingStation();
    int getDirtLevel(char direction=STAY) const;
    int isWall(char direction) const;
    double batteryRemaining() const;
    void handleNextStep(char nextStep);
    const std::string OUTPUT_FILE_NAME = "output.txt";
};

#endif