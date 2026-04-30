# Lab 01 – Sensor Data Acquisition

## 📖 Description
This project implements a modular Arduino sketch for sensor data acquisition using the Arduino Nano RP2040 Connect.

It was developed as part of the Applied Physics for Computing course at IPBeja.

---

## 🎯 Objectives
- Simultaneous acquisition of data from multiple sensors
- Timestamp association with each reading
- Implementation of moving average filtering
- Visualization in Serial Monitor or Serial Plotter
- Modular structure for future extension (Arduino Cloud)

---

## 📡 Hardware Used
- Arduino Nano RP2040 Connect

### Sensors:
- IMU (Accelerometer + Gyroscope + Temperature)
- Microphone (RMS and Peak sound levels)

---

## ⚙️ Features
- Real-time data acquisition
- Moving average filtering (noise reduction)
- Timestamped readings
- Multiple output modes:
  - Serial Monitor
  - Serial Plotter
  - Cloud-ready structure

---

## 🧱 Code Structure

### Main File
- `lab01_sensores.ino`
  - Contains `setup()` and `loop()`
  - Defines configuration parameters (output mode and filter window)

### Configuration Files
- `header.h` → libraries and constants
- `types.h` → data structures (SensorReadings, Sample)

### Sensor Modules
- `sensor_imu.h / sensor_imu.cpp` → IMU sensor functions
- `sensor_mic.h / sensor_mic.cpp` → microphone functions

### Core Functions
- `functions.h / functions.cpp`
  - Sensor reading
  - Data processing
  - Moving average calculation
  - Output handling (Monitor / Plotter / future Cloud)

---

## ▶️ How to Use

### 1. Select Output Mode
Edit the following line in `lab01_sensores.ino`:

```cpp
int outputMode = MODE_SERIAL_MONITOR;
```

Options:
- `MODE_SERIAL_MONITOR`
- `MODE_SERIAL_PLOTTER`
- `MODE_CLOUD` (prepared for next task)

---

### 2. Adjust Filter Window
```cpp
int windowSize = DEFAULT_WINDOW;
```

You can change the value (e.g., 5, 10, 20).

---

### 3. Upload Code
1. Open Arduino IDE  
2. Select board: Arduino Nano RP2040 Connect  
3. Select correct COM port  
4. Upload code  
5. Open Serial Monitor or Serial Plotter  

---

## ⚙️ System Behavior

### Data Acquisition
- All sensors are read simultaneously at each sampling cycle

### Timestamp
- Each reading is associated with a timestamp (milliseconds)

### Filtering
- Moving average filter applied to sensor data
- Uses a circular buffer with configurable window size

### Output
- Serial Monitor → detailed labeled output  
- Serial Plotter → tabulated numeric values  
- Cloud → prepared for Task 2 integration  

---

## ⚠️ Notes
- Code is modular and easy to extend
- Prepared for Arduino Cloud integration (Lab 02)
- Designed for clarity, maintainability, and scalability

---

## 🔒 Security
Sensitive information such as device IDs, API keys, and secret keys are NOT included in this repository.

---

## 📌 Academic Context
This project corresponds to Lab 01 of the Applied Physics for Computing course and serves as the foundation for further development in subsequent tasks (Cloud integration and data analysis).
