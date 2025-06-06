# Smart Wheelchair: Gesture-Controlled Mobility and Health Monitoring

**A compact, all-in-one ESP32-based solution for accessible mobility and real-time health tracking**

This project integrates three ESP32 microcontrollers and an Arduino board to create a smart wheelchair system controlled by glove-based gestures and equipped with comprehensive health monitoring. Designed for individuals with mobility challenges, it combines intuitive control, obstacle avoidance, and vital signs tracking, making it ideal for assistive technology, educational projects, or telemedicine.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Applications](#applications)
- [Repository Structure](#repository-structure)
- [System Requirements](#system-requirements)
- [Installation and Setup](#installation-and-setup)
- [Usage](#usage)
- [Customization](#customization)
- [Advanced Integrations](#advanced-integrations)
- [Troubleshooting](#troubleshooting)
- [Maintenance](#maintenance)
- [Security Considerations](#security-considerations)
- [Testing](#testing)
- [Performance Metrics](#performance-metrics)
- [FAQ](#faq)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [Acknowledgments](#acknowledgments)
- [Revision History](#revision-history)

## Overview
The Complete Smart Wheelchair System comprises four components:
- **Gesture Controller (gesture_controller.ino)**: An ESP32 with an MPU6050 gyroscope embedded in a glove detects hand tilts to send movement commands (forward, backward, left, right, stop) to the wheelchair via WebSocket over a Wi-Fi network.
- **Wheelchair Base (wheelchair_base.ino)**: An ESP32 controls the wheelchair’s motors, avoids obstacles using an ultrasonic sensor, and hosts a web server for manual control.
- **Health Monitor (health_monitor.ino)**: An ESP32 collects heart rate and SpO2 from a MAX30102 sensor and ECG/temperature from an Arduino via Serial2. It sends email alerts for high temperatures (>100°F) and uploads data to [ThingSpeak](https://thingspeak.com).
- **Arduino Sensor Hub (arduino_sensor_hub.ino)**: An Arduino reads ECG from an AD8232 sensor and temperature from an MLX90614 sensor, sending data to the health monitor in CSV format.

The gesture controller and wheelchair base are integrated via Wi-Fi, while the health monitor and Arduino sensor hub work together to provide real-time health data, enhancing user safety.

## Features
- **Gesture Controller**:
  - Intuitive glove-based control using MPU6050 gyroscope tilt detection.
  - Real-time WebSocket communication for reliable command transmission.
  - Automatic gyroscope calibration for accuracy.
- **Wheelchair Base**:
  - Motor control for five movements (forward, backward, left, right, stop).
  - Ultrasonic sensor for obstacle avoidance (stops at <20 cm).
  - Responsive web interface for manual control via a browser.
- **Health Monitor**:
  - Monitors heart rate, SpO2 (MAX30102), ECG, and temperature (Arduino).
  - Email alerts for high temperature (>100°F or 37.78°C).
  - Data upload to ThingSpeak every 20 seconds for remote monitoring.
- **Arduino Sensor Hub**:
  - Collects ECG (AD8232) and ambient/object temperature (MLX90614).
  - Sends data to ESP32 health monitor via Serial in CSV format.

## Applications
- **Assistive Technology**: Empowers individuals with mobility challenges through gesture control and health monitoring.
- **Educational Projects**: Demonstrates IoT, sensor integration, wireless communication, and embedded systems.
- **Robotics**: Showcases gesture-controlled mobility with obstacle avoidance.
- **Telemedicine**: Enables remote health monitoring for caregivers or medical professionals.

## Repository Structure
```
SmartWheelchair/
├── gesture_controller/
│   ├── gesture_controller.ino
│   └── README_controller.md
├── wheelchair_base/
│   ├── wheelchair_base.ino
│   └── README_wheelchair.md
├── health_monitor/
│   ├── health_monitor.ino
│   └── README_health_monitor.md
├── arduino_sensor_hub/
│   ├── arduino_sensor_hub.ino
│   └── README_arduino.md
├── docs/
│   ├── circuit_diagram.md
│   └── pin_mapping_guide.md
├── README.md
├── LICENSE
└── .gitignore
```

## System Requirements

### Hardware
| Component | Gesture Controller | Wheelchair Base | Health Monitor | Arduino Sensor Hub |
|-----------|--------------------|-----------------|----------------|--------------------|
| **Board** | ESP32 (e.g., ESP32-WROOM-32) | ESP32 | ESP32 | Arduino Uno or compatible |
| **Sensors** | MPU6050 (I2C: SDA to GPIO21, SCL to GPIO22) | Ultrasonic (TRIG to GPIO25, ECHO to GPIO26) | MAX30102 (I2C: SDA to GPIO21, SCL to GPIO22), Arduino via Serial2 (GPIO16, GPIO17) | AD8232 (LO+ to pin 10, LO- to pin 11, OUTPUT to A0), MLX90614 (I2C: SDA to A4, SCL to A5) |
| **Actuators** | None | Motor driver (e.g., L298N) to GPIO13, GPIO12, GPIO14, GPIO27 | None | None |
| **Power Supply** | 5V, 1A (USB or battery) | 7-12V, 2A (for motors) | 5V, 1A | 5V, 1A |
| **Other** | Glove to mount ESP32 and MPU6050 | Wheelchair chassis with DC motors | Finger clip for MAX30102 | ECG electrodes for AD8232 |

### Software
- [Arduino IDE](https://www.arduino.cc/en/software) (version 2.x or higher)
- Libraries:
  - Gesture Controller: [Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050), [WebSocketsClient](https://github.com/martinh/Arduino-Websockets)
  - Wheelchair Base: `WebServer`, `WebSocketsServer` (included in ESP32 core)
  - Health Monitor: [MAX30102](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library), [ESP_Mail_Client](https://github.com/mobizt/ESP-Mail-Client), [ThingSpeak](https://github.com/mathworks/thingspeak-arduino)
  - Arduino Sensor Hub: [Wire](https://www.arduino.cc/reference/en/language/functions/communication/wire/), [Adafruit_MLX90614](https://github.com/adafruit/Adafruit_MLX90614)

## Installation and Setup

### 1. Gesture Controller (gesture_controller.ino)
- **Hardware**:
  - Mount ESP32 and MPU6050 in a glove.
  - Connect MPU6050: SDA to GPIO21, SCL to GPIO22, VCC to 3.3V, GND to GND.
  - Power with a 5V, 1A source (e.g., USB or battery).
- **Software**:
  1. Install [Arduino IDE](https://www.arduino.cc/en/software).
  2. Add ESP32 board support:
     - Go to `File` > `Preferences`, add `https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json` to `Additional Boards Manager URLs`.
     - In `Tools` > `Board` > `Boards Manager`, search for `esp32` and install the package by Espressif Systems.
  3. Install `Adafruit_MPU6050` and `WebSocketsClient` via `Sketch` > `Include Library` > `Manage Libraries`.
  4. Open `gesture_controller.ino`, select `ESP32 Dev Module`, and upload.

### 2. Wheelchair Base (wheelchair_base.ino)
- **Hardware**:
  - Connect motor driver (e.g., L298N): IN1 to GPIO13, IN2 to GPIO12, IN3 to GPIO14, IN4 to GPIO27.
  - Connect ultrasonic sensor: TRIG to GPIO25, ECHO to GPIO26.
  - Attach DC motors to the wheelchair chassis.
  - Power with a 7-12V, 2A battery (ensure motor driver and ESP32 share GND).
- **Software**:
  1. Open `wheelchair_base.ino` in Arduino IDE.
  2. Upload to the ESP32 (no additional libraries needed; `WebServer` and `WebSocketsServer` are in ESP32 core).

### 3. Health Monitor (health_monitor.ino)
- **Hardware**:
  - Connect MAX30102 sensor via I2C: SDA to GPIO21, SCL to GPIO22.
  - Connect Arduino to Serial2: Arduino TX to ESP32 RX2 (GPIO16), Arduino RX to ESP32 TX2 (GPIO17).
  - Power with a 5V, 1A source.
- **Software**:
  1. Install `MAX30102`, `ESP_Mail_Client`, and `ThingSpeak` libraries via Arduino IDE.
  2. Update WiFi credentials (`ssid`, `password`) in `health_monitor.ino`.
  3. Create a [ThingSpeak](https://thingspeak.com) channel and update `channelID` and `writeAPIKey` in the code`.
  4. Configure email settings (`SENDER_EMAIL`, `SENDER_APP_PASSWORD`, `RECIPIENT_EMAIL`) using a Gmail app password from [Google Support](https://support.google.com/support/answer/185833).
  5. Upload `health_monitor.ino` to the ESP32.

### 4. Arduino Sensor Hub (arduino_sensor_hub.ino)
- **Hardware**:
  - Connect AD8232 ECG sensor: LO+ to pin 10, LO- to pin 11, OUTPUT to A0.
  - Connect MLX90614 temperature sensor: SDA to A4, SCL to A5.
  - Connect Arduino to ESP32 health monitor: Arduino TX (pin 1) to ESP32 RX2 GPIO16), Arduino RX (pin 0) to ESP32 TX2 (GPIO17).
  - Power with a 5V, 1A0 source.
- **Software**:
  1. Open `Wire.h` and `Adafruit_MLX90614` libraries via `Arduino IDE`.
  2. Upload `arduino_sensor_hub.ino` to the Arduino board (e.g., Uno).

## Usage

### Gesture-Controlled Mobility
1. **Power On**:
   - Power on the wheelchair base ESP32 to create the "SmartWheelchair" Wi-Fi network (SSID: "SmartWheelchair", password: "12345678").
   - Power on the gesture controller ESP32; it connects to "SmartWheelchair" and calibrates the gyroscope (keep still for 5 seconds).
2. **Control**:
   - Tilt the glove to send movement commands:
     - Forward: Tilt forward (>0.5 rad/s on Y-axis).
     - Backward: Tilt backward (<-0.5 rad/s on Y-axis).
   - Left: Tilt left: Tilt left (>0.5 rad/s on X-axis).
   - Right: Tilt right (>0.5 rad/s on X-axis).
     - Stop: No tilt.
   - The wheelchair moves based on commands, stopping if an obstacle is detected (<20 cm).
3. **Manual Control**:
   - Connect a device (e.g., smartphone) to the "SmartWheelchair" network.
   - Access the web interface at `http://192.168.4.1` for manual control via buttons (Forward, Backward, Left, Right, Stop).

### Health Monitoring
- Ensure the MAX30102 sensor is worn on the finger) and the MAX30102 ECG electrodes are attached to the skin).
- The health monitor ESP32 connects to a specified WiFi network (e.g., WiFi "Robo Tech" Valley").
- Collects data:
  - **MAX30102**: Heart rate and SpO2.
  - **Arduino**: ECG, ambient temperature, and body temperature).
- Sends email alerts if body temperature exceeds 100°F (37.78°C).
- Uploads data to the ThingSpeak website every 20 seconds for remote monitoring].

## Customization
- **Gesture Controller**:
  - Adjust the gyroscope thresholds (e.g., `gyroX > 0.5`) in `gesture_controller.ino` for sensitivity.
  - Change the WebSocket server IP/port if needed.
- **Wheelchair Base**:
  - Modify motor control logic for different motor drivers in `wheelchair_base.ino`.
  - Adjust obstacle detection distance to (default: 20 cm).
- **Health Monitor**:
  - Change temperature alert threshold or (default: 100°F).
  - Adjust data upload frequency to (default: 20 seconds).
- **Arduino Sensor Hub**:
  - Modify pin assignments or Serial baud rate in `arduino_sensor_hub.ino`.

## Advanced Integrations
- **IoT Platforms**: Integrate with [Home Assistant](https://www.home-assistant.io) or [Adafruit IO](https://io.adafruit.com) for centralized control and visualization.
- **Additional Sensors**: Add a camera (e.g., ESP32-CAM) or GPS to the wheelchair for navigation or tracking.
- **Health Enhancements**: Include blood pressure sensors or develop a mobile app for real-time alerts.
- **Voice Control**: Integrate with [Google Assistant](https://assistant.google.com) or [Alexa](https://developer.amazon.com/assistant/alexa) for voice-activated control or status checks.
- **Machine Learning**: Deploy lightweight ML models (e.g., via TensorFlow Lite) for predictive health analysis or fall detection.

## Troubleshooting

| Component | Possible Cause | Solution |
|-----------|----------------|----------|
| **Gesture Controller** | Not connecting to WiFi | Verify "SmartWiFi" SSID and password in `gesture_controller`.ino`. Check WiFi range. |
| **Wheelchair Base** | Motors not moving | Check motor driver connections, power supply (7-12V), and WebSocket connection. |
| **Health Monitor** | Not uploading data | Verify WiFi credentials, ThingSpeak API key, and internet connectivity. |
| **Health Monitor** | No email alerts | Ensure Gmail app password is correct. Check serial monitor for errors. |
| **Arduino Sensor Hub** | No data received | Verify Serial connections (Arduino TX to ESP32 RX2). Ensure RX2). Check baud rate (115200).

## Maintenance
- **Firmware Updates**: Re-upload code via Arduino IDE for new features or fixes.
- **Log Monitoring**: Use serial monitor (115200 baud) for debugging and status updates.
- **Hardware Checks**: Inspect sensor and motor connections for wear or loose wiring.
- **Battery Management**: Regularly charge or replace batteries, especially for the wheelchair base.

## Security Considerations
- **WiFi Security**: Use a strong password for the "SmartWiFi" network to prevent unauthorized access.
- **Email Security**: Use a dedicated email account for alerts to avoid exposing personal credentials.
- **Data Privacy**: Keep ThingSpeak channels private for sensitive health data. Consider encryption for data transmission.
- **Web Interface**: The web interface lacks authentication; add [Basic Authentication](https://example.com) for public networks.

## Testing
- **Gesture Controller**: Tilt the glove and verify commands in the serial monitor (e.g., `[SENT: SENT] Command: F`).
- **Wheelchair Base**: Test motor responses via web interface and obstacle detection by placing objects within 20 cm.
- **Health Monitor**: Verify heart rate, SpO2, and ECG/temperature data on ThingSpeak. Trigger a high-temperature alert (>100°F).
- **Arduino Sensor Hub**: Check ESP32 serial monitor to confirm CSV data (e.g., `500,25.0,36.5,1`) is received.

## Performance Metrics
| Metric | Value |
|--------|-------|
| Gesture Command Rate | Every 100ms |
| Obstacle Detection | <100ms |
| Health Data Upload | Every 20s |
| Email Alert Delay | ~2-5s |
| Arduino Data Rate | Every 10ms |

## FAQ
- **How do I change the WiFi network for the health monitor?**  
  Update `ssid` and `password` in `health_monitor`.ino` and re-upload.
- **Why isn't the wheelchair moving?**  
  Check motor connections, power supply, and WebSocket communication in the serial monitor.
- **How do I set up ThingSpeak?**  
  Create a channel on [ThingSpeak](https://thingspeak.com) and update the API key in `health_monitor`.ino`.
- **What is the role of the Arduino?**  
  It collects ECG and temperature data, sending it to the health monitor via Serial2 in CSV format.
- **Can I use a different glove design?**  
  Yes, ensure the ESP32 and MPU6050 are securely mounted and powered.

## Contributing
Contributions are welcome! To contribute:
1. Fork the repository on [GitHub](https://github.com).
2. Create a branch (`git checkout -b feature/your-feature`).
3. Make and test changes to your feature branch.
4. Commit with clear messages (`git commit -m "Add feature description"`).
   5. Push to your fork (`git push origin feature/your-feature`).
   6. Submit a pull request with a detailed description.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact
For support, open an issue on [GitHub](https://github.com) or email [sm.abdulhasib.bd@gmail.com](mailto:sm.abdulhasib.bd@gmail.com).

## Acknowledgments
- [ESP32 Arduino Core](https://github.com/espressif/arduino_code-esp32)
- [Adafruit MPU6050 Library](https://github.com/adafruit/Adafruit_MPU6050)
- [WebSocketsClient Library](https://github.com/martinh/mArduino-Websockets)
- [ESP Mail Client Library](https://github.com/mMobizt/HArduinoESP_Mail_Client)
- [ThingSpeak Library](https://github.com/mathworks/Thingspeak-arduino)
- [Adafruit MLX90614 Library](https://github.com/adafruit/Adafruit_MLX90614)
- [SparkFun MAX301 Library](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library)

## Revision History
- **Last Updated**: 03:35 AM +06, Saturday, June 7, 2025
