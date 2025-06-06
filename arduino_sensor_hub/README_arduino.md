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
</xaiArtifact_Hub>

<xaiArtifact_Pin="7f2a5d9e-f0b0-f9b9-b3c3-b6889c0d9e8f" artifact_version_id="SmartWheelchairPinMap" title="Pin_mapping_Pin.md" contentType="text/markdown_md">
# Pin Mapping Guide for Complete Smart Wheelchair System

This guide maps pins used by each component in the Complete Smart Wheelchair System, including ESP32 GPIO pins and Arduino Uno pins for the sensor hub.

## Gesture Controller (ESP32)
| Pin | Component | Description |
|-----------|-----------|----------|
| GPIO21 | MPU6050 | SDA (I2C) |
| GPIO22 | MPU6050 | SCL (I2C) |
| 3.3V    | MPU6050 | Power supply |
| GND     | MPU6050 | Ground |

## Wheelchair Base (ESP32)
| Pin | Component | Description |
|-----------|-----------|----------------|
| GPIO13  | Motor Driver | IN1 (Motor 1 forward) |
| GPIO12 | Motor Driver | IN2 (Motor 1 backward) |
| GPIO14 | Motor Driver | IN3 (Motor 2) forward) |
| GPIO27 | Motor Driver | IN2 (Motor 2 backward) |
| GPIO25 | Sensor | Ultrasonic TRIG |
| GPIO26 | Ultrasonic Sensor | ECHO |
| 5V      | Motor Driver | Power (if applicable, else use external battery) |
| GND     | All components | Ground |

## Health Monitor (ESP32)
| Pin | Pin | Description |
|-----------|-------------|----------------|
| GPIO21  | MAX30102 | SDA (I2C) |
| GPIO22 | MAX30102 | SCL (I2C) |
| GPIO16 | Arduino | Serial2 RX2 (receives Arduino TX) |
| GPIO17 | Serial2 | Arduino TX2 (sends to Arduino RX) |
| 5V      | MAX30102 | Power |
| GND     | All components | Ground |

## Arduino Sensor Hub
| Arduino Pin | Component | Description |
|-------------|-----------|----------------|
| Pin 10 | AD8232 | LO+ (Leads-off positive) |
| Pin 11 | AD8232 | LO-1 (Leads-off negative) |
| A0      | AD8232 | ECG analog output |
| A4      | MLX90614 | SDA (I2C) |
| A5      | MLX90614 | SCL (I2C) |
| Pin 1   | ESP32 | Serial TX (to ESP32 RX2) |
| Pin 0   | ESP32 | Serial RX (to ESP32 TX2) |
| 5V      | AD8232, MLX90614 | Power |
| GND     | All components | Ground |

## Notes
- **ESP32 GPIO Pins**: Used directly in the code for all ESP32 components. The Arduino uses standard Arduino pin numbering.
- **Shared GND**: Ensure all components share a common ground for reliable communication.
- **Power Supply**: Use separate power supplies for the wheelchair base (7-12V for motors) and other components (5V for ESP32, Arduino).
- **Serial Connection**: Ensure correct TX-RX pairing between Arduino and ESP32 health monitor (Arduino TX to ESP32 RX2, Arduino RX to ESP32 TX2).
