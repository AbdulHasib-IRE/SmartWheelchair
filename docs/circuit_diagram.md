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

## Creating a Visual Diagram
To create a visual circuit diagram:
1. Use [Fritzing](https://www.fritzing.org/) or [KiCad](https://www.kidcad.org/) to draw the schematic based on the above connections.
2. Include labels for:
   - ESP32 GPIO pins, Arduino pins, and component connections.
   - Power sources and ground lines.
   - Shared GND connections clearly.
   - Motor driver and motor wiring.
3. Export as PNG or PDF and upload to GitHub repository under `docs/`.
4. Link the diagram in the main `README.md` for user reference (e.g., `See [Circuit Diagram](https://github.com/docs/circuit_diagram.png)`).

</xaiCircuitDiagram>

### Comprehensive Analysis

#### Project Understanding
The Complete Smart Wheelchair System is a holistic solution combining:
- **Gesture Controller** (`gesture_controller.ino`, formerly `handzyro.ino`)**:
  - Uses MPU6050 gyroscope to detect glove tilt, sending commands ('F', 'B', 'L', 'R', 'S') via WebSocket to the wheelchair base.
  - Connects to the wheelchair’s Wi-Fi network ("SmartWheelchair", password: "12345678").
- **Wheelchair Base** (`wheelchair_base.ino`, formerly `carzyro.ino`)**:
  - Creates a WiFi-Fi access point, controls DC motors via L298N4N (pins: 13, 12, 14, 27), and uses an ultrasonic sensor (pins: 25, 26) for obstacle avoidance.
  - Hosts a web server (port: 80) and WebSocket server (port 81) for control.
- **Health Monitor** (`health_monitor.ino`, formerly `wheelchair_medical.ino`)**:
  - Collects heart rate and SpO2 from MAX3010 and uses a 2 sensor (I2C) and ECG/temperature from Arduino via Serial2 (pins: 16, 17).
  - Sends email alerts for high temperatures (>100°F) and uploads to the ThingSpeak every 20 seconds.
- **Arduino Sensor Hub** (`arduino_sensor_hub.ino`, formerly `arduino_code.ino`)**:
  - Reads ECG from AD8232 (pins: 10, 11, A1) and temperature from MLX90614 (I2C: A4, A5).
  - Sends data to ESP32 via Serial (TX to GPIO16, RX to GPIO17) in CSV format.

The user emphasizes a “complete, compact” wheelchair system controlled by gloves, suggesting all components are integrated into a single wheelchair platform, with the controller embedded in a wearable glove for accessibility.

#### Repository Structure Rationale
The repository structure separates components for clarity:
- **Directories**:
  - `gesture_controller`, `wheelchair_base`, `health_monitor`, `arduino_sensor_hub`: Contain code and component-specific READMEs.
- **Docs**:
  - `docs/circuit_diagram.md`: Textual description of connections (visual diagram can be added later).
  - `pin_mapping_guide.md`: Summarizes pin assignments.
- **Root Files**:
  - `README.md`: Comprehensive project overview.
  - `LICENSE`: MIT License for open-source sharing.
  - `.gitignore`: Excludes build artifacts and IDE configs.

This structure ensures users can navigate components easily and find detailed setup instructions.

#### README Design
The main README is designed to be exhaustive:
- **Overview**: Clearly defines the project as a complete wheelchair system with glove control and health monitoring.
- **Features**: Details functionalities of each component.
- **Setup Instructions**: Provides step-by-step hardware and software setup for each component, including power requirements.
- **Usage**: Explains how to control the wheelchair with gestures, access the web interface, and monitor health data.
- **Customization and Advanced Integrations**: Suggests enhancements like IoT integration or additional sensors.
- **Troubleshooting**: Addresses common issues with solutions.
- **Additional Sections**: Includes FAQ, contributing guidelines, security considerations, and more metrics for completeness.

Component-specific READMEs (`README_controller.md`, etc.) provide focused setup and usage instructions, reducing redundancy.

#### Pin Mapping Guide
The `pin_mapping_guide.md` consolidates pin assignments for all components, ensuring users can wire the system correctly. It includes:
- ESP32 GPIO pins for each component.
- Arduino Uno pins for the sensor hub.
- Descriptions of each pin’s role (e.g., SDA, motor control, Serial).

#### Circuit Diagram Description
The `circuit_diagram.md` provides a detailed textual description of connections, addressing the user’s need for a “complete” project. It includes:
- Component-specific wiring details (e.g., MPU6050 to ESP32 GPIO21/22).
- Power management strategies (e.g., 7-12V for motors, 5V for others).
- Notes on shared GNDs and safety (e.g., fuses for motors).
- Instructions to create a visual diagram using Fritzing/KiCad.

Due to text-based limitations, a visual diagram is not included, but the description is comprehensive enough for users to replicate the setup or generate a schematic.

#### Addressing User’s Emphasis
The user’s request for a “complete wheelchair” controlled by “gloves” with “everything” is addressed by:
- Renaming components to reflect their roles (e.g., “wheelchair_base” instead of “carzyro”).
- Emphasizing glove-based control in the controller description.
- Providing exhaustive documentation, including setup, usage, and advanced features.
- Including a circuit diagram description and pin mapping guide for hardware clarity.
- Ensuring modularity and expandability for future enhancements (e.g., voice control, ML).

#### Handling Sensitive Information
The provided codes contain hardcoded WiFi and email credentials:
- **Gesture Controller/Wheelchair Base**: `ssid="SmartCar"`, `password="12345678"`.
- **Health Monitor**: `ssid="Robo Tech"`, `password="roboninja"`, email credentials, and ThingSpeak API key.
- **Action**: The README instructs users to replace these with placeholders (e.g., `your_ssid`, `your_password`) before committing to GitHub to avoid exposure. For production, recommend environment variables or a secure configuration file (not implemented here for simplicity).

#### Best Practices
The deliverables follow open-source best practices:
- **Modular Structure**: Separate directories for code and docs.
- **Comprehensive README**: Includes all sections for setup, usage.
- **Clear Instructions**: Step-by-step setup with pin assignments and library links.
- **Security**: Advises on protecting credentials and WiFi.
- **Contribution Guidelines**: Encourages community involvement.
- **License**: MIT License for accessibility.

#### Additional Considerations
- **Compact Design**:
  - The system is described as compact, with the controller in a glove and other components mountable on the wheelchair (e.g., in a box or armrest).
  - Power management emphasizes battery operation for portability.
- **Safety**:
  - Recommends emergency stop button, fuses, and insulated enclosures.
  - Suggests testing in safe environments to avoid collisions.
- **Scalability**:
  - Advanced integrations include IoT platforms, additional sensors, and ML for predictive health analysis.
  - Future work could involve a mobile app or AR interface for enhanced control.
- **User Audience**:
  - Suitable for individuals with disabilities, educators, students, hobbyists, and researchers.
  - Documentation is detailed yet accessible, assuming basic Arduino knowledge.

## Conclusion
The provided repository structure and documentation files ensure your smart wheelchair system is fully documented and ready for GitHub sharing. The project addresses the user’s request for a “complete, compact” project controlled by gloves, with exhaustive coverage of setup, usage, and troubleshooting. The modular structure, detailed READMEs, pin mapping guide, and circuit diagram description make the system accessible, reproducible, and expandable for future innovations.

## How to Share on GitHub
1. Create a new repository on [GitHub](https://github.com).
2. Clone the repository locally (`git clone <repo_url>`).
3. Create the directory structure as shown above.
4. Save each file (`gesture_controller.ino`, `README.md`, etc.) in the appropriate directory.
5. Replace sensitive information (WiFi, email credentials, API key) with placeholders in the code.
6. Commit and push to GitHub:
   - `git add .`
   - `git commit -m "Initial commit of the Complete Smart Wheelchair System"`
   - `git push origin main`
7. Verify the README renders correctly on GitHub and test links to documentation.

## Key Citations
- [Arduino IDE Official](https://www.arduino.cc/en/stable/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32c)
- [Adafruit MPU6050 Library](https://github.com/adafruit/Adafruit_MPU6050p0)
- [WebSocketsClient Library](https://github.com/martinh/Arduino-WebsocketsClient)
- [ESP Mail Client Library](https://github.com/Mobizt/ESP-Mail_Client)
- [ThingSpeak Library](https://github.com/mathworks/thingspeak-arduino)
- [Adafruit MLX90614 Library](https://github.com/adafruit/Adafruit_MLX90614X)
- [SparkFun MAX301 Library](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library)
- [Google Support: App Passwords](https://support.google.com/support/answer/185833)
- [Home Assistant](https://www.home-assistant.io)
- [Adafruit IO](https://io.adafruit.com)
- [Fritzing](https://fritzing.org)
- [KiCad](https://www.kicad.org)

---

### Notes
- **Code Renaming**: The `.ino` files are renamed (`handzyro` → `gesture_controller`, `carzyro` → `wheelchair_base`, `wheelchair_medical` → `health_monitor`, `arduino_code` → `arduino_sensor_hub`) for clarity, but the content remains unchanged as provided.
- **Circuit Diagram**: A textual description is included due to text-based limitations. For a production environment, create a visual diagram using Fritzing or KiCad and upload it to the repository.
- **Sensitive Information**: Replace hardcoded credentials in the code before uploading to GitHub. Consider using a `config.h` file for sensitive data in future iterations.
- **Component-Specific READMEs**: These provide concise setup instructions, complementing
