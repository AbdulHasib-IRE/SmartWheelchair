# Circuit Diagram Description for Smart Wheelchair

This markdown file describes the circuit connections for the Complete Smart Wheelchair System. Due to the limitations of text-based markdown, the diagram is provided as a textual description. For a graphical representation, consider using tools like Fritzing or KiCad to create a visual diagram based on this guide.

## Overview
The system includes three ESP32 microcontrollers and an Arduino board, each with specific components connected via I2C, Serial, or GPIO pins. Below are the detailed connections for each component.

### 1. Gesture Controller (ESP32)
- **Components**:
  - ESP32 development board (e.g., ESP32-WROOM-32)
  - MPU6050 gyroscope sensor
- **Connections**:
  - MPU6050 SDA → ESP32 GPIO21
  - MPU6050 SCL → ESP32 GPIO22
  - MPU6050 VCC → ESP32 V3
.3V
  - MPU6050 GND → ESP32 GND
- **Power**:
  - ESP32 via USB (5V, 1A) or battery (e.g., Li-ion with 3.3V regulator for MPU6050)
- **Notes**:
  - Mount ESP32 and MPU6050 in a glove, ensuring freedom of movement for tilt detection.
  - Use short I2C wires to minimize noise.

### 2. Wheelchair Base (ESP32)
- **Components**:
  - ESP32 development board
  - Motor driver (e.g., with L298N)
  - Two DC motors for wheelchair chassis
  - Ultrasonic sensor (HC-SR04, or similar)
- **Connections**:
  - Motor Driver IN1 → ESP32 GPIO13
  - Motor Driver IN2 → ESP32 GPIO12
  - Motor Driver IN3 → ESP32 GPIO14
  - Motor Driver IN4 → ESP32 GPIO27
  - Motor Driver OUT1, OUT2 → Motor 1 (left or right wheel)
  - Motor Driver OUT3, OUT4 → Motor 2 (opposite wheel)
  - Ultrasonic TRIG → ESP32 GPIO25
  - Ultrasonic ECHO → ESP32 GPIO26
  - Motor Driver VCC → 7-12V battery (e.g., 12V lead-acid)
  - Motor Driver GND → ESP32 GND, battery GND
  - ESP32 VIN → 5V regulator (from battery, or separate 5V source)
- **Power**:
  - 7-12V, 2A battery for motors; 5V, 1A for ESP32 (shared GND, required).
- **Notes**:
  - Secure motor driver and motors to the wheelchair chassis.
  - Test motor polarity to ensure correct movement directions.
  - Use a fuse or circuit breaker for motor power safety.

### 3. Health Monitor (ESP32)
- **Components**:
  - ESP32 development board
  - MAX3015 pulse oximeter sensor
  - Connection to Arduino sensor hub
- **Connections**:
  - MAX3015 SDA → ESP32 GPIO21
  - MAX3015 SCL → ESP32 GPIO22
  - MAX3015 VCC → ESP32 5V
  - MAX30105 GND → ESP32 GND
  - ESP32 RX2 (GPIO16) → Arduino TX (pin 1)
  - ESP32 TX2 (GPIO17) → Arduino RX (pin 0)
- **Power**:
  - 5V, 1A via USB or battery for ESP32 and MAX30105.
- **Notes**:
  - Place MAX30105 in a finger clip for heart rate and SpO2 readings.
  - Ensure Serial2 wires are secure for reliable data transfer from Arduino.

### 4. Arduino Sensor Hub
- **Components**:
  - Arduino Uno or compatible board
  - AD8232 ECG sensor
  - MLX90614 non-contact temperature sensor
- **Connections**:
  - AD8232 LO+ → Arduino Pin 10
  - AD8232 LO- → Arduino Pin 11
  - AD8232 OUTPUT → Arduino A0
  - MLX90614 SDA → Arduino A4
  - MLX90614 SCL → Arduino A5
  - AD8232 VCC → Arduino 5V
  - MLX90614 VCC → Arduino 5V
  - AD8232 GND, MLX90614 GND → Arduino GND
  - Arduino TX (pin 1) → ESP32 RX2 (GPIO16)
  - Arduino RX (pin 0) → ESP32 TX2 (GPIO17)
- **Power**:
  - 5V, 1A via USB or external source for Arduino.
- **Notes**:
  - Attach ECG electrodes to the patient’s skin (e.g., chest or arms, per AD8232 guidelines).
  - Position MLX90614 for accurate body temperature readings (e.g., forehead).
  - Share GND with ESP32 health monitor for Serial communication.

### Power Management
- **Gesture Controller** and **Health Monitor**:
  - Use separate 5V or USB power sources (e.g., power banks) for portability.
- **Wheelchair Base**:
  - Use a high-capacity battery (e.g., 12V, 7Ah lead-acid) for motors, with a 5V regulator for ESP32.
- **Arduino Sensor Hub**:
  - Power via USB or shared 5V health monitor (if feasible).
- **Shared GND**: Connect all component grounds to avoid communication issues.

### Additional Notes
- **Safety**:
  - Use insulated wires and enclosures to protect components.
  - Include an emergency stop button on the wheelchair.
  - Test power stability to prevent brownouts during motor operation.
- **Integration**:
  - Ensure all components are mounted securely on the wheelchair (e.g., controller in glove, MAX30105 on armrest, Arduino/ESP32 in a weather-resistant box).
  - **Debugging**: Use separate USB connections to monitor serial output during development.
- **Scalability**:
  - Add LEDs or a small OLED display for status feedback (e.g., battery level, WiFi status).
  - Consider a charging dock for all components.


