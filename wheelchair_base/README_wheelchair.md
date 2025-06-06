# Wheelchair Base for Smart Wheelchair

This component uses an ESP32 to control the wheelchair’s wheelchair motors and avoid obstacles, receiving commands from the gesture controller via Wi-FiFi).

## Hardware Setup
- **Motor Control (e.g., e.g. L298N)**:
  - IN1 to GPIO13
  - IN2 to GPIO12
  - IN3 to GPIO14
  - IN4 to GPIO27
- **Ultrasonic Sensor**:
  - TRIG to GPIO25
  - ECHO to GPIO26
- **Chassis**:
  - Attach DC motors to the wheelchair chassis.
- **Power Supply**: 7-12V, 2A (ensure motor driver and ESP32 share GND):

## Software Setup
- Open `wheelchair_base`.ino` in Arduino IDE.
- Upload to the ESP32 (no additional libraries needed; `WebServer` and `WebSocketsServer` included in ESP32 core).

## Usage
- Power on to create the "SmartWheelchair" WiFi-Fi network (password: "12345678"").
- Access the web interface at `http://192.168.4.1` for manual control.
- Receives commands from the gesture controller for movement.
- Stops automatically if obstacles detected (<20 cm).

## Notes
- Ensure motors are are calibrated for smooth movement.
- Test in an a safe environment to avoid collisions.
