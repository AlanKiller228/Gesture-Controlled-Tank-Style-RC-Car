# 🚜 Gesture-Controlled Tank-Style RC Car

A wireless, gesture-controlled robotic vehicle operated via a wearable motion-tracking glove, enabling intuitive tilt-based navigation.

---

## 📌 Features

- **Gesture Control:** Wearable glove tracks hand orientation for tilt-based steering and throttle.
- **Low-Latency Wireless:** Fast peer-to-peer communication using the **ESP-NOW** protocol (no Wi-Fi router needed).
- **Differential Drive Logic:** Smooth tank-style movement calculations for precise turning.
- **Dead-Zone Filtering:** Custom filtering to compensate for raw sensor noise and motor friction thresholds.

---

## 🛠️ Hardware Requirements

| Component | Description | Quantity |
| :--- | :--- | :---: |
| **ESP32 Microcontroller** | Transmitter (Glove) & Receiver (Car) | 2 |
| **MPU6050 IMU** | 6-axis Gyro/Accelerometer sensor | 1 |
| **L298N Motor Driver** | Dual H-Bridge driver for DC motors | 1 |
| **DC Motors & Tank Chassis**| Tracks and driving motors | 1 set |
| **Power Supply** | Li-Ion / LiPo Batteries for motors & ESP32 | 1-2 |

---

## 💻 Tech Stack & Protocols

- **Language:** C++ / Arduino Framework
- **Communication:** ESP-NOW Protocol, I2C Interface (MPU6050)
- **IDE:** Arduino IDE / PlatformIO

---

## 📂 Repository Structure

```text
├── transmitter_glove/    # Code for ESP32 + MPU6050 transmitter glove
├── receiver_car/         # Code for ESP32 + L298N motor driver car
└── README.md
