#include "House.h"
#include "VacuumCleaner.h"
#include "Algorithm.h"
#include <iostream>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time>
#include <stdexcept> // For std::exception

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>\n";
        return 1;
    }

    try {
        House house(argv[1]);
        VacuumCleaner vacuum_cleaner(&house);
        Algorithm algorithm(&vacuum_cleaner);

        // Main loop for vacuum cleaner operation
        while (!vacuum_cleaner.isMissionCompleted() && !vacuum_cleaner.isDead() && vacuum_cleaner.getStepsTaken() < house.getMaxSteps()) {
            // Check if the vacuum cleaner is on the docking station
            if (vacuum_cleaner.isAtDockingStation()) {
                // Stay and charge until fully charged
                while (vacuum_cleaner.getBatteryLevel() < vacuum_cleaner.getMaxBatterySteps()) {
                    try {
                        vacuum_cleaner.charge(1); // Charge 1 step
                        vacuum_cleaner.clean();   // Clean if there's dirt at the docking station
                        vacuum_cleaner.move('S'); // Stay in place
                    } catch (const std::exception& e) {
                        std::cerr << "Recoverable error during charging and cleaning at docking station: " << e.what() << std::endl;
                    }
                }
            }

            // If the battery level is equal to the path size or path size + 1, start backtracking
            if (vacuum_cleaner.getBatteryLevel() == vacuum_cleaner.getPathSize() || vacuum_cleaner.getBatteryLevel() == vacuum_cleaner.getPathSize() + 1) {
                // Implement backtracking logic here
                while (!vacuum_cleaner.isAtDockingStation() && vacuum_cleaner.getBatteryLevel() > 0) {
                    try {
                        char next_step = vacuum_cleaner.backtrack();
                        vacuum_cleaner.move(next_step);
                    } catch (const std::exception& e) {
                        std::cerr << "Recoverable error during backtracking: " << e.what() << std::endl;
                        break; // If backtracking fails, break out of the loop
                    }
                }
                continue; // Skip the rest of the loop and check conditions again
            }

            // Decide the next step using the algorithm
            char next_step;
            try {
                next_step = algorithm.decideNextStep();
            } catch (const std::exception& e) {
                std::cerr << "Recoverable error in deciding next step: " << e.what() << std::endl;
                next_step = 'S'; // Default to staying in place on error
            }

            try {
                vacuum_cleaner.move(next_step);

                // If staying in place, clean the dirt
                if (next_step == 'S') {
                    vacuum_cleaner.clean();
                }
            } catch (const std::exception& e) {
                std::cerr << "Recoverable error during movement/cleaning: " << e.what() << std::endl;
            }
        }

        // Output results to a file or console
        std::cout << "Cleaning mission completed!" << std::endl;
        std::cout << "Total steps taken: " << vacuum_cleaner.getStepsTaken() << std::endl;
        std::cout << "Remaining battery: " << vacuum_cleaner.getBatteryLevel() << std::endl;
        std::cout << "Dirt remaining in house: " << house.getTotalDirt() << std::endl;
        std::cout << (vacuum_cleaner.isDead() ? "Vacuum cleaner is dead." : "Vacuum cleaner is alive.") << std::endl;
        std::cout << (vacuum_cleaner.isMissionCompleted() ? "Mission succeeded!" : "Mission failed.") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Unrecoverable error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown unrecoverable error occurred." << std::endl;
    }

    return 0;
}
