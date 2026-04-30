# Arduino IoT Data Monitoring System

## 📖 Description
This project was developed as part of the Applied Physics for Computing course at IPBeja.

It implements a complete system for sensor data acquisition, cloud visualization, and data analysis using Arduino and Python.

---

## ⚙️ Features
- Real-time sensor data acquisition from multiple sensors
- Moving average filtering for noise reduction
- Timestamped sensor readings
- Visualization via Serial Monitor and Serial Plotter
- Integration with Arduino Cloud (Dashboard)
- Python data analysis using Jupyter Notebook
- Statistical analysis (mean, median, min, max, standard deviation)
- Alert system for threshold detection
- Export of processed data to Excel

---

## 📡 Hardware
- Arduino Nano RP2040 Connect

### Sensors:
- IMU (Accelerometer + Gyroscope + Temperature)
- Microphone (RMS and Peak sound levels)

---

## 🧠 System Overview

### 1. Data Acquisition (Arduino)
- Reads all sensors simultaneously
- Applies moving average filtering
- Sends data via Serial and Cloud

### 2. Cloud Visualization (Arduino Cloud)
- Real-time dashboard
- Graphs and indicators
- Alerts and LED control

### 3. Data Processing (Python)
- Reads serial data
- Generates graphs and statistics
- Detects anomalies
- Exports results to Excel

---

## 📊 Dashboard
(Add screenshot here)


arduino/
lab01_sensores/

python/
(data analysis notebooks)

dashboard/
(dashboard screenshots)

docs/
(academic task descriptions)



---

## 🎯 Learning Outcomes
- IoT system integration
- Embedded systems programming (Arduino)
- Data acquisition and filtering
- Cloud-based monitoring systems
- Data analysis with Python
- Visualization and reporting

---

## ⚠️ Security Note
Credentials such as device IDs, API keys, and secret keys are NOT included in this repository for security reasons.

---

## 📌 Academic Context
This project combines three laboratory tasks:
- Sensor data acquisition
- Arduino Cloud dashboard development
- Data processing and visualization with Python
---

## 📁 Project Structure
