# Lab 02 – Arduino Cloud Dashboard

## 📖 Description
This project extends the sensor acquisition system developed in Lab 01 by integrating the Arduino Nano RP2040 Connect with Arduino Cloud.

It was developed as part of the Applied Physics for Computing course at IPBeja.

---

## 🎯 Objectives
- Send sensor data from Arduino to Arduino Cloud
- Create a real-time dashboard for data visualization
- Display sensor readings using charts and widgets
- Implement alerts for predefined sensor thresholds
- Control the Arduino built-in LED from the cloud dashboard
- Include a timestamp for sensor readings

---

## 📡 Hardware Used
- Arduino Nano RP2040 Connect

### Sensors:
- IMU (Accelerometer + Gyroscope + Temperature)
- Microphone (RMS sound level)

---

## ⚙️ Features
- Real-time sensor reading
- Arduino Cloud integration
- Dashboard with graphs and indicators
- LED control using a switch/button
- Alert logic for temperature and sound thresholds
- Timestamped readings
- Map/location widget in the dashboard

---

## 🧠 System Overview
The Arduino reads sensor data locally and sends selected values to Arduino Cloud.

The dashboard displays:
- Total acceleration
- Sound RMS level
- Temperature
- Timestamp in seconds
- LED status and control
- Sensor location map

---

## 📊 Dashboard Screenshot
The dashboard screenshot is stored in the `dashboard/` folder of the main repository.

---

## 📁 Files
- `projeto-fisica_jan11a.ino` → main Arduino Cloud sketch
- `thingProperties.h` → Arduino Cloud generated properties
- `arduino_secrets.example.h` → example credentials file
- `sketch.json` → Arduino sketch configuration

---

## 🔒 Security
Real credentials, device IDs, API keys, Wi-Fi passwords, and secret keys are not included in this repository.

Use `arduino_secrets.example.h` as a template and create a local `arduino_secrets.h` file when running the project.

---

## 📌 Academic Context
This project corresponds to Lab 02 of the Applied Physics for Computing course and prepares the system for the final data acquisition and analysis task using Python.