
# Pin Mapping Guide for Smart Wheelchair 

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
