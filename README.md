# Vacuum Cleaner Robot Project

## Contributors

- Amit Rockach, Id: 322853813
- Michal Ling, Id: 206507923

## Overview

This project simulates a vacuum cleaner robot that navigates through a house, cleans dirt, and returns to its docking station to recharge. The project is implemented in C++ and consists of several key components: the main application logic, a representation of the house, the vacuum cleaner, a controller to manage the cleaning process, and utilities for handling file I/O and logging.

## Files

### `myrobot.cpp`

This is the main entry point of the program. It initializes the components, starts the cleaning process, and handles any exceptions that occur.

### `Point.h` and `Point.cpp`

These files define the `Point` class, which represents a point in a 2D coordinate system. It includes methods for moving and comparing points.

### `VacuumCleaner.h` and `VacuumCleaner.cpp`

These files define the `VacuumCleaner` class, which simulates the behavior of the vacuum cleaner robot. It handles movement, battery management, and location tracking.

### `House.h` and `House.cpp`

These files define the `House` class, which represents the house layout and dirt levels. It includes methods for checking walls, managing dirt levels, and visualizing the house.

### `Algorithm.h`, `Algorithm.cpp` (and `abstract_algorithm.h`)

These files define the `Algorithm` component, which is an implementation of the `AbstractAlgorithm` abstract class. It encompasses the decision-making process of the vacuum cleaner robot. It determines how the vacuum cleaner moves from one point to another while avoiding obstacles and making sure the vacuum cleaner doesn't run out of battery.

### `FileDataExtractor.h` and `FileDataExtractor.cpp`

These files define the `FileDataExtractor` class, which reads and extracts data from the input file, needed for initializing the house and vacuum cleaner.

### `MySimulator.h` and `MySimulator.cpp`

These files define the `MySimulator` class, which manages the overall cleaning process. It includes the main loop for the cleaning algorithm, handles battery recharging, and creates an output file with the results.

### `BatteryMeterImpl.h`, `BatteryMeterImpl.cpp` (and `battery_meter.h`)

This file defines the `BatteryMeterImpl` class, which is an implementation of the `BatteryMeter` abstract class. The class is used by the Algorithm class as a sensor providing the state of the vacuum cleaner's battery.

### `DirtSensorImpl.h`, `DirtSensorImpl.cpp` (and `dirt_sensor.h`)

This file defines the `DirtSensorImpl` class, which is an implementation of the `DirtSensor` abstract class. The class is responsible for detecting the level of dirt at the vacuum cleaner's current location.

### `WallsSensorImpl.h`, `WallsSensorImpl.cpp` (and `wall_sensor.h`)

This file defines the `WallsSensorImpl` class, which is an implementation of the `WallsSensor` abstract class. The class is responsible for detecting the presence of walls in different directions around the vacuum cleaner.

### `Direction.h`, `Direction.cpp` and `enums.h`

This file contains utility functions for converting between `Direction` and `Step` enumerations, as well as functions for obtaining string representations of these enums defined in `enums.h`.

### `General.h`

This file contains general definitions and constants used throughout the project.

### `Utils.h` and `Utils.cpp`

These files consist of small functions that are not directly associated with any class within the project.

### `Config.h` and `Config.cpp`

These files define the `Config` class, implemented as a singleton to centralize and manage program configuration settings effectively. The class provides a single instance accessible through the `getInstance()` method, ensuring consistent access to configuration data throughout the application. It handles the loading and parsing of configuration data from a file named `config.txt`, where each line follows the `<key>=<value>` format strictly. Valid key-value pairs are stored in an unordered map within the singleton instance, allowing efficient access and modification of configuration parameters across different parts of the program. Notably, essential keys such as `logFileName` are predefined with default fallback values from `General.h` in cases where these keys are absent or incorrectly formatted in the configuration file.

### `Logger.h` and `Logger.cpp`

These files define the `Logger` class, implemented as a singleton to handle logging functionalities within the program. The class includes methods for logging informational messages (`logInfo`) and error messages (`logError`). It supports logging to standard output (`logToStdout`) and standard error (`logToStderr`), with the exception that logs won't print to `stdout` if `useVisualizer=true` in `config.txt` so the user can see the visualization. Additionally, the logger logs to a file specified in the configuration under the key `logToFile` (in case the key doesn't exist the program has a default fallback value and will log to `application.log`).

## Error Handling

The program handles various errors, such as invalid file format, out-of-bounds coordinates, and runtime exceptions. If an error occurs, it logs the error message and terminates the program gracefully.

## How to Build - Linux

1. Ensure you have a C++ compiler installed (e.g., `g++`).
2. Compile the project using the following command:
   ```sh
   make
   ```
3. Run the executable with an input file:
   ```sh
   ./myrobot inputfile.txt
   ```

## Solution Approach

The solution implements a vacuum cleaner robot that autonomously navigates a house, cleans dirt, and returns to its docking station to recharge. MySimulator class manages the cleaning process, starting with the initialization of the vacuum cleaner and house layout. Inside MySimulator's loop, it checks if the vacuum cleaner has completed cleaning the house, or is at the docking station. If not, using the Algorithm class, the vacuum cleaner decides its next move using DFS based on battery levels, obstacle detection, and remaining dirt levels. The algorithm includes a backtracking mechanism, ensuring the robot can efficiently return to the docking station for recharging. When the simulation finishes, results are logged and saved to an output file.

## Input and Output File Format

According to Assignment 2's instructions
