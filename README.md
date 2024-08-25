# Vacuum Cleaner Robot Project

## Contributors

- Amit Rockach, Id: 322853813
- Michal Ling, Id: 206507923

## Overview

This project simulates a vacuum cleaner robot that navigates through a house, cleans dirt, and returns to its docking station to recharge. The project is implemented in C++ and consists of several key components: the main application logic, a representation of the house, the vacuum cleaner, a controller to manage the cleaning process, and utilities for handling file I/O and logging.

## Files

### algorithms
### `Algorithm.h`, `Algorithm.cpp` (and `AlgorithmRegistration.h`)

These files define the `Algorithm` component, which implements the core logic of how the vacuum cleaner robot navigates the house. `Algorithm.h` and `Algorithm.cpp` contain the implementation of the `Algorithm` class, derived from the `AbstractAlgorithm` interface. `AlgorithmRegistration.h` handles the registration of different algorithm implementations with the system.

### `DeterministicAlgorithm.h`, `DeterministicAlgorithm.cpp`

These files define the `DeterministicAlgorithm` class, which implements a deterministic algorithm based on DFS search. If there's more than one step possible at a time, the algorithm will choose the first. 

### `RandomAlgorithm.h`, `RandomAlgorithm.cpp`
These files define the `RandomAlgorithm` class, which implements a random algorithm based on DFS search. If there's more than one step possible at a time, the algorithm will choose randomly.

### common
### `AlgorithmRegistrar.h`, `AlgorithmRegistrar.cpp`
These files implement the `AlgorithmRegistrar` class, which is responsible for registering and managing different algorithm implementations. It allows dynamic loading and switching of algorithms during the simulation.

### `BatteryMeterImpl.h`, `BatteryMeterImpl.cpp` (and `battery_meter.h` from simulator folder)

This file defines the `BatteryMeterImpl` class, which is an implementation of the `BatteryMeter` abstract class. The class is used by the Algorithm class as a sensor providing the state of the vacuum cleaner's battery.

### `Direction.h`, `Direction.cpp` and `enums.h`

This file contains utility functions for converting between `Direction` and `Step` enumerations, as well as functions for obtaining string representations of these enums defined in `enums.h`.

### `DirtSensorImpl.h`, `DirtSensorImpl.cpp` (and `dirt_sensor.h` from simulator folder)

This file defines the `DirtSensorImpl` class, which is an implementation of the `DirtSensor` abstract class. The class is responsible for detecting the level of dirt at the vacuum cleaner's current location.

### `WallsSensorImpl.h`, `WallsSensorImpl.cpp` (and `wall_sensor.h` from simulator folder)

This file defines the `WallsSensorImpl` class, which is an implementation of the `WallsSensor` abstract class. The class is responsible for detecting the presence of walls in different directions around the vacuum cleaner.

### `General.h`

This file contains general definitions and constants used throughout the project.

### `Utils.h` and `Utils.cpp`

These files consist of small functions that are not directly associated with any class within the project.

### `Logger.h` and `Logger.cpp`

These files define the `Logger` class, implemented as a singleton to handle logging functionalities within the program. The class includes methods for logging informational messages (`logInfo`) and error messages (`logError`). It supports logging to standard output (`logToStdout`) and standard error (`logToStderr`). The program log file is `application.log`.

### `Point.h` and `Point.cpp`

These files define the `Point` class, which represents a point in a 2D coordinate system. It includes methods for moving and comparing points.

### simulator

### `myrobot.cpp`

This is the main entry point of the program. It initializes the components, starts the cleaning process, and handles any exceptions that occur.

### `AlgorithmWrapper.h`, `AlgorithmWrapper.cpp` 

These files define the `AlgorithmWrapper` class, which serves as an interface between the core simulation logic and the specific algorithm implementations. It abstracts away the details of the algorithms and ensures they can be interchanged seamlessly.

### `VacuumCleaner.h` and `VacuumCleaner.cpp`

These files define the `VacuumCleaner` class, which simulates the behavior of the vacuum cleaner robot. It handles movement, battery management, and location tracking.

### `House.h` and `House.cpp`

These files define the `House` class, which represents the house layout and dirt levels. It includes methods for checking walls and managing dirt levels.

### `HouseWrapper.h`, `HouseWrapper.cpp`

These files define the `HouseWrapper` class, which abstracts away the details of house files.

### `FileDataExtractor.h` and `FileDataExtractor.cpp`

These files define the `FileDataExtractor` class, which reads and extracts data from the input file, needed for initializing the house and vacuum cleaner.

### `MySimulator.h` and `MySimulator.cpp`

These files define the `MySimulator` class, which manages the overall cleaning process. It includes the main loop for the cleaning algorithm, handles battery recharging, and creates an output file with the results.

### `ThreadController.h` and `ThreadController.cpp`

These files define the `ThreadController` class, which manages multithreading within the simulation. It ensures that different components of the simulation, such as the vacuum cleaner and sensor updates, run concurrently and efficiently, improving the overall performance of the system.


## Error Handling

The program handles various errors, such as invalid file format, out-of-bounds coordinates, and runtime exceptions. If an error occurs, it logs the error message and terminates the program gracefully.

## How to Build - Linux

1. Ensure you have a C++ compiler installed (e.g., `g++`).
2. Compile the project using the following command:
   ```sh
   ./build.sh
   ```
3. Run the executable with an input file:
   ```sh
   ./myrobot -house_path=<house folder path> -algo_path=<algo folder path>
   ```

## Input and Output File Format

According to Assignment 3's instructions.
In case the program reaches timeout, the letter `T` will represent it in the output file. 

## Example inputs and output

In the folder `tests` there is an `inputs` folder with house files (`*.house` files).
Additionally, you can see an example `summary.csv` under the same `tests` directory
