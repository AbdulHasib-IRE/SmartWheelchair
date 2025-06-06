## Sensor Hub for Smart Wheelchair

This Arduino board collects ECG and temperature data, sending it to the health monitor via a Serial monitor.

## Hardware Setup
- **AD8232 ECG Sensor**:
  - LO+ to pin 10
  - LO- to pin 11
  - OUTPUT to A0
- **MLX90614 Temperature Sensor**:
  - SDA to A4
  - SCL to A5
- **Connection to ESP32**:
  - Arduino TX (pin 1) to ESP32 RX2 (GPIO16)
  - Arduino RX (pin 0) to ESP32 TX2 (GPIO17)
- **Power Supply**: 5V, 1A

## Software Setup
- Install Arduino IDE and libraries (`Wire`, `Adafruit_MLX90614`).
- Upload `arduino_controller_hub`.ino` to Arduino (e.g., Uno).

## Usage
- Reads ECG and temperature data continuously.
- Sends data to ESP32 in CSV format: `ecgValue,ambientTemp,objectTemp,leadStatus`.

## Notes
- Ensure ECG electrodes are properly attached to skin.
- Check I2C connections for MLX90614 stability.
