#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include <string>
#include <stdexcept>

class House {
public:
    House(const std::string& filename);

    int getMaxBatterySteps() const;
    int getMaxSteps() const;
    int getDirtLevel(int x, int y) const;
    void setDirtLevel(int x, int y, int level);
    bool isWall(int x, int y) const;
    int getDockingX() const;
    int getDockingY() const;
    int getTotalDirt() const;

private:
    void loadHouse(const std::string& filename);

    std::vector<std::vector<int>> house_map;
    int docking_x, docking_y;
    int max_battery_steps;
    int max_steps;
};

#endif // HOUSE_H
