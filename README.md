## Class Diagram

### House

- **Fields:**
  - `house_map: vector<vector<int>>`
  - `docking_x: int`
  - `docking_y: int`
  - `max_battery_steps: int`
  - `max_steps: int`
- **Methods:**
  - `House(filename: string)`
  - `getMaxBatterySteps(): int`
  - `getMaxSteps(): int`
  - `getDirtLevel(x: int, y: int): int`
  - `setDirtLevel(x: int, y: int, level: int)`
  - `isWall(x: int, y: int): bool`
  - `getDockingX(): int`
  - `getDockingY(): int`
  - `getTotalDirt(): int`
  - `loadHouse(filename: string)`

### VacuumCleaner (inherits from Sensors)

- **Fields:**
  - `house: House*`
  - `x: int`
  - `y: int`
  - `battery: int`
  - `stepsTaken: int`
  - `missionCompleted: bool`
  - `dead: bool`
  - `path: stack<char>`
- **Methods:**
  - `VacuumCleaner(house: House*)`
  - `move(direction: char)`
  - `clean()`
  - `charge(steps: int)`
  - `isMissionCompleted(): bool`
  - `isDead(): bool`
  - `getStepsTaken(): int`
  - `getBatteryLevel(): int`
  - `getMaxBatterySteps(): int`
  - `isAtDockingStation(): bool`
  - `getPathSize(): int`
  - `backtrack(): char`
  - `getDirtLevel(): int`
  - `isWall(direction: char): bool`
  - `updateMissionStatus()`

### Algorithm

- **Fields:**
  - `vacuumCleaner: VacuumCleaner*`
  - `directions: vector<char> {'N', 'E', 'S', 'W'}`
- **Methods:**
  - `Algorithm(vacuumCleaner: VacuumCleaner*)`
  - `decideNextStep(): char`
  - `getValidMoves(): vector<char>`

### House File Format

The house file format is a text file that represents a grid layout of the house, including dirt levels, walls, and the docking station for the vacuum cleaner. Each character in the file corresponds to a specific element in the grid.

#### File Structure

- Each line represents a row in the grid.
- Each character in a line represents a cell in the grid.
- Valid characters:
  - `'0'` to `'9'`: Represents the amount of dirt in the cell (0 means no dirt).
  - `'W'`: Represents a wall.
  - `'D'`: Represents the docking station for the vacuum cleaner (dirt level is implicitly 0).
  - Any other character will cause an error when loading the file.

#### Default Settings

- The maximum battery steps (`max_battery_steps`) is set to 100 by default.
- The maximum steps (`max_steps`) is set to 1000 by default.

### Example

Consider the following house file:
1 1 2 3
2 3 W 1
0 1 D 2
3 W 1 0

#### Explanation

- The house grid is a 4x4 matrix.
- The first row has dirt levels 1, 1, 2, and 3.
- The second row has dirt levels 2, 3, a wall, and 1.
- The third row has dirt levels 0, 1, and 2 with the docking station at the third position.
- The fourth row has dirt levels 3, a wall, 1, and 0.
