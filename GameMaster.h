#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include "House.h"
#include "VacuumCleaner.h"
#include "Algorithm.h"

class GameMaster {
public:
    GameMaster(House& house, VacuumCleaner& vacuumCleaner, int maxSteps, int stepsTaken = 0, bool missionCompleted = false, bool gameOver = false);
    void run();

private:
    House house;
    VacuumCleaner vacuumCleaner;
    Algorithm algorithm;
    int maxSteps;
    int stepsTaken;
    bool missionCompleted;
    bool gameOver;
    static const std::vector<char> directions;


    void loadGameSettings(const std::string &filename);
    void initGeneralGameSettings(std::ifstream& file);
    void initVacuumCleaner(std::ifstream& file);
    void initMaxSteps(std::ifstream& file);
    void initHouse(std::ifstream& file);
    void gameLoop();
    void handleDockingStation();
    void handleBacktracking();
    void handleNextStep();
};

const std::vector<char> GameMaster::directions = {'S', 'W', 'N', 'E'};

#endif // GAMEMASTER_H
