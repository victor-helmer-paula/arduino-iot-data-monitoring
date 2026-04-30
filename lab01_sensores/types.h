#ifndef TYPES_H
#define TYPES_H

typedef struct {
  float ax, ay, az;
  float gx, gy, gz;
  float temp;
  float soundRMS;
  float soundPeak;
} SensorReadings;

typedef struct {
  unsigned long timestamp;
  SensorReadings values;
} Sample;

#endif
