#include <Wire.h>
#include <Adafruit_MLX90614.h>

// AD8232 ECG Sensor Pins
const int LO_PLUS_PIN = 10;  // LO+ pin
const int LO_MINUS_PIN = 11; // LO- pin
const int ECG_OUTPUT_PIN = A0; // ECG analog output

// MLX90614 Temperature Sensor (GY-906)
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(115200); // Use hardware Serial (TX=1, RX=0)
  
  // Initialize AD8232 leads-off detection
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);
  
  // Initialize MLX90614
  if (!mlx.begin()) {
    Serial.println("GY-906 not found!");
    while (1); // Halt if sensor fails
  }
  
  delay(1000); // Stabilization time
}

void loop() {
  // Read ECG value (0-1023)
  int ecgValue = analogRead(ECG_OUTPUT_PIN);
  
  // Read temperatures (°C)
  float ambientTemp = mlx.readAmbientTempC();
  float objectTemp = mlx.readObjectTempC();
  
  // Check ECG lead status (1=connected, 0=disconnected)
  int leadStatus = 1;
  if ((digitalRead(LO_PLUS_PIN) == 1) || (digitalRead(LO_MINUS_PIN) == 1)) {
    leadStatus = 0;
  }
  
  // Send data to ESP32 via Serial in CSV format
  Serial.print(ecgValue);
  Serial.print(",");
  Serial.print(ambientTemp);
  Serial.print(",");
  Serial.print(objectTemp);
  Serial.print(",");
  Serial.println(leadStatus);
  
  delay(10); // Prevent data flooding
}
