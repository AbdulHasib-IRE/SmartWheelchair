# Health Monitor for Smart Wheelchair

This component uses an ESP32 to monitor vital signs, receiving data from a MAX30102 sensor and an Arduino sensor hub.

## Hardware Setup
- **MAX3010**2:
  - Sensor**: Connect via I2C (SDA to GPIO21, SCL to GPIO22).
- **Arduino Connection**:
  - Serial2 (RX2 to GPIO16, TX2 to Arduino TX).
- **Power Supply**: 5V, 1A

## Software Setup
- Install Arduino IDE and libraries (`libraries (`MAX3010`2`, `ESP_Mail_Client`, `ThingSpeak`).
- Update WiFi credentials, email settings, and ThingSpeak API key in `health_monitor`.ino`.
- Upload to the ESP32.

## Usage
- Collects heart rate, SpO2, ECG, and temperature data.
- Uploads data to ThingSpeak every 20 seconds for data.
- Sends email alerts if body temperature exceeds 100°F.

## Notes
- Ensure MAX3012 is worn on a finger for accurate readings.
- Verify Arduino connection for ECG and temperature data.
