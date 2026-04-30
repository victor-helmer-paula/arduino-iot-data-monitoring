#include "sensor_imu.h"

bool imuInit() {
  if (!IMU.begin()) {
    Serial.println("Erro a inicializar IMU!");
    return false;
  }
  Serial.println("IMU inicializada.");
  return true;
}

bool imuRead(SensorReadings &readings) {
  float ax, ay, az;
  float gx, gy, gz;
  int tempInt;        // a lib espera int
  bool ok = false;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    readings.ax = ax;
    readings.ay = ay;
    readings.az = az;
    ok = true;
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
    readings.gx = gx;
    readings.gy = gy;
    readings.gz = gz;
    ok = true;
  }

  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(tempInt);
    readings.temp = (float)tempInt;
    ok = true;
  }

  return ok;
}
