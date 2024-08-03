#ifndef WALL_SENSOR_H_
#define WALL_SENSOR_H_

#include "../common/enums.h"

class WallsSensor {
public:
	virtual ~WallsSensor() {}
	virtual bool isWall(Direction d) const = 0;
};

#endif  // WALL_SENSOR_H_
