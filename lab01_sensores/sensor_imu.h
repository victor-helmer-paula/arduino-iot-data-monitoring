#ifndef SENSOR_IMU_H
#define SENSOR_IMU_H

#include "header.h"

bool imuInit();
bool imuRead(SensorReadings &readings);

#endif
