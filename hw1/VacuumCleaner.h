#ifndef VACUUMCLEANER_H
#define VACUUMCLEANER_H

#include "House.h"
#include "Sensors.h"
#include <stack>

class VacuumCleaner : public Sensors {
public:
    VacuumCleaner(House* house);

    void move(char direction);
    void clean();
    void charge(int steps);
    bool isMissionCompleted() const;
    bool isDead() const;
    int getStepsTaken() const;
    int getBatteryLevel() const override;
    int getMaxBatterySteps() const override;
    bool isAtDockingStation() const;
    int getPathSize() const;

    // Backtracking
    char backtrack();

    // Sensors interface implementation
    int getDirtLevel() const override;
    bool isWall(char direction) const override;

private:
    House* house;
    int x, y;
    int battery;
    int stepsTaken;
    bool missionCompleted;
    bool dead;
    std::stack<char> path;

    void updateMissionStatus();
};

#endif // VACUUMCLEANER_H
