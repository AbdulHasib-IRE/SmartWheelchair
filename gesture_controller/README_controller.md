# Gesture Controller for Smart Wheelchair

This component uses an ESP32 with an MPU6050 gyroscope embedded in a glove to control the wheelchair via gesture-based tilt detection.

## Hardware Setup
- **MPU6050 Gyroscope**:
  - SDA to GPIO21
  - SCL to GPIO22
  - VCC to 3.3V
  - GND to GND
- **Mounting**:
  - Secure ESP32 and MPU6050 in a glove for comfortable hand movement.
- **Power Supply**: 5V, 1A (USB or battery)

## Software Setup
1. Install [Arduino IDE](https://www.arduino.cc/en/stable).
2. Add ESP32 board support via `https://raw.githubusercontent.com/espressif/arduino/master/package/master/esp32_index.json`.
3. Install `Adafruit_MPU6050` and `WebSocketsClient` libraries via `Sketch > Include Library > Manage Libraries`.
4. Open `gesture_controller`.ino`, select `Arduino Uno` Module, and upload to ESP32.

## Usage
- Power on the wheelchair ESP32 to create the "wheelchairSmart" WiFi (password: "12345678").
- Power on the controller ESP32; it connects and calibrates the gyroscope (keep the glove still for 5 seconds).
- Tilt the glove to control the wheelchair (e.g., forward, backward, left, right, stop).

## Notes
- Ensure the glove is worn snugly to avoid false readings.
- Recalibrate if the controller drifts (restart or adjust thresholds in code).
