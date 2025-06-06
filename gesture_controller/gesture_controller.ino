#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
WebSocketsClient webSocket;

// Wi-Fi Credentials (Connect to Car's AP)
const char* ssid = "GyroCar";
const char* password = "12345678";

// Calibration offsets
float gyroXoffset = 0, gyroYoffset = 0, gyroZoffset = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("[ERROR] MPU6050 not found!");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  
  // Calibrate gyro
  calibrateGyro();
  
  // Connect to Car's Wi-Fi
  Serial.println("\nConnecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to WebSocket
  webSocket.begin("192.168.4.1", 81, "/"); // Car's AP IP
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply calibration and print data
  float gyroX = g.gyro.x - gyroXoffset;
  float gyroY = g.gyro.y - gyroYoffset;
  
  Serial.print("Raw X: ");
  Serial.print(g.gyro.x, 4);
  Serial.print(" | Calib X: ");
  Serial.print(gyroX, 4);
  Serial.print(" | Raw Y: ");
  Serial.print(g.gyro.y, 4);
  Serial.print(" | Calib Y: ");
  Serial.print(gyroY, 4);
  Serial.println();

  // Send commands
  char cmd = 'S';
  if (gyroY > 0.5)       cmd = 'F';
  else if (gyroY < -0.5) cmd = 'B';
  else if (gyroX > 0.5)  cmd = 'R';
  else if (gyroX < -0.5) cmd = 'L';
  
  String payload = String(cmd);
  if (webSocket.sendTXT(payload)) {
    Serial.print("[SENT] Command: ");
    Serial.println(cmd);
  } else {
    Serial.println("[ERROR] Failed to send command!");
  }

  delay(100);
}

// Calibrate gyro offsets
void calibrateGyro() {
  Serial.println("\n=== CALIBRATION STARTED ===");
  Serial.println("Keep the sensor STILL for 5 seconds...");
  
  const int numSamples = 500;
  sensors_event_t a, g, temp;
  
  for (int i = 0; i < numSamples; i++) {
    mpu.getEvent(&a, &g, &temp);
    gyroXoffset += g.gyro.x;
    gyroYoffset += g.gyro.y;
    gyroZoffset += g.gyro.z;
    
    if (i % 50 == 0) {
      Serial.print("Progress: ");
      Serial.print((i * 100) / numSamples);
      Serial.println("%");
    }
    delay(10);
  }
  
  gyroXoffset /= numSamples;
  gyroYoffset /= numSamples;
  gyroZoffset /= numSamples;
  
  Serial.println("\n=== CALIBRATION RESULTS ===");
  Serial.print("Gyro X Offset: ");
  Serial.println(gyroXoffset, 6);
  Serial.print("Gyro Y Offset: ");
  Serial.println(gyroYoffset, 6);
  Serial.print("Gyro Z Offset: ");
  Serial.println(gyroZoffset, 6);
  Serial.println("==========================\n");
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] Disconnected from car!");
      break;
    case WStype_CONNECTED:
      Serial.println("[WS] Connected to car!");
      break;
  }
}
