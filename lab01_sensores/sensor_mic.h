#ifndef SENSOR_MIC_H
#define SENSOR_MIC_H

#include "header.h"

bool micInit();
void micUpdate();
void micComputeFeatures(SensorReadings &readings);

#endif
