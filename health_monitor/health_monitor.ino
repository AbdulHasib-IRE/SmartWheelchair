#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <ESP_Mail_Client.h>
#include "MAX30105.h"
#include "heartRate.h"

// WiFi Credentials
const char* ssid = "############";
const char* password = "&&&&&&&&&&&&&&";

// ThingSpeak Settings
unsigned long channelID = ***********;
const char* writeAPIKey = "$$$$$$$$$$$$$$$$";
WiFiClient client;

// Email Configuration
#define SENDER_EMAIL "@@@@@@@@@@@@@"
#define SENDER_APP_PASSWORD "******************"
#define RECIPIENT_EMAIL "sm.abdulhasib.bd@gmail.com"
SMTPSession smtp;
bool emailSent = false;

// MAX30102 Sensor
MAX30105 max30102;
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
float beatsPerMinute;
int beatAvg;
long lastBeat = 0;
float spo2 = 0.0;

// SpO2 Calculation Variables
#define SAMPLING_RATE 400
#define AVERAGING 4
#define ADC_RANGE 4096
#define SAMPLE_COUNT 25
long redBuffer[SAMPLE_COUNT];
long irBuffer[SAMPLE_COUNT];

// Arduino Data Variables
int ecgValue = 0;
float ambientTempC = 0;
float objectTempC = 0;
int leadStatus = 1;

// Timing Control
unsigned long lastUpload = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX2=16, TX2=17

  // Initialize MAX30102
  if (!max30102.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found!");
    while (1);
  }
  max30102.setup();
  max30102.setPulseAmplitudeRed(0x0A);

  // Configure sensor settings
  byte ledBrightness = 0x1F;
  byte sampleAverage = AVERAGING;
  byte ledMode = 2;
  max30102.setup(ledBrightness, sampleAverage, ledMode, SAMPLING_RATE, 411, ADC_RANGE);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  
  ThingSpeak.begin(client);
}

void sendEmailAlert() {
  ESP_Mail_Session session;
  session.server.host_name = "smtp.gmail.com";
  session.server.port = 465;
  session.login.email = SENDER_EMAIL;
  session.login.password = SENDER_APP_PASSWORD;

  SMTP_Message message;
  message.sender.email = SENDER_EMAIL;
  message.subject = "HIGH TEMPERATURE ALERT!";
  message.addRecipient("", RECIPIENT_EMAIL);

  float objectTempF = (objectTempC * 9.0/5.0) + 32;
  String content = "Object Temperature Alert!\n\n";
  content += "Current Temperature: " + String(objectTempF, 2) + "°F\n";
  content += "(" + String(objectTempC, 2) + "°C)\n\n";
  content += "System Time: " + String(millis()/1000) + " seconds";
  
  message.text.content = content.c_str();

  if (!smtp.connect(&session)) return;
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Email failed");
  } else {
    Serial.println("Alert sent");
    emailSent = true;
  }
}

void parseSensorData(String rawData) {
  int values[4] = {0};
  int index = 0;
  int lastIndex = 0;

  for (int i = 0; i < 4; i++) {
    index = rawData.indexOf(',', lastIndex);
    if (index == -1 && i < 3) return;
    
    String part = (i == 3) ? 
                 rawData.substring(lastIndex) :
                 rawData.substring(lastIndex, index);
                 
    if (i == 0) values[i] = part.toInt();         // ECG
    else if (i == 3) values[i] = part.toInt();    // Lead status
    else values[i] = part.toFloat() * 100;        // Temp with 2 decimal precision
    
    lastIndex = index + 1;
  }

  ecgValue = values[0];
  ambientTempC = values[1] / 100.0;
  objectTempC = values[2] / 100.0;
  leadStatus = values[3];
}

void checkHeartRate(long irValue) {
  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute > 20 && beatsPerMinute < 255) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
}

void calculateSpO2(long red, long ir) {
  static byte spo2Count = 0;
  
  // Shift buffer
  for (int i = SAMPLE_COUNT-1; i > 0; i--) {
    redBuffer[i] = redBuffer[i-1];
    irBuffer[i] = irBuffer[i-1];
  }
  
  // Add new values
  redBuffer[0] = red;
  irBuffer[0] = ir;

  if (spo2Count++ < SAMPLE_COUNT) return;

  // Calculate DC components
  float redDC = 0, irDC = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    redDC += redBuffer[i];
    irDC += irBuffer[i];
  }
  redDC /= SAMPLE_COUNT;
  irDC /= SAMPLE_COUNT;

  // Calculate AC components
  float redAC = 0, irAC = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    redAC += pow(redBuffer[i] - redDC, 2);
    irAC += pow(irBuffer[i] - irDC, 2);
  }
  redAC = sqrt(redAC/SAMPLE_COUNT);
  irAC = sqrt(irAC/SAMPLE_COUNT);

  // Calculate SpO2
  float ratio = (redAC/redDC) / (irAC/irDC);
  spo2 = 110.0 - 18.0 * ratio;
  spo2 = constrain(spo2, 70.0, 100.0);
}

void uploadToThingSpeak() {
  ThingSpeak.setField(1, ecgValue);
  ThingSpeak.setField(2, ambientTempC);
  ThingSpeak.setField(3, objectTempC);
  ThingSpeak.setField(4, leadStatus);
  ThingSpeak.setField(5, beatAvg);
  ThingSpeak.setField(6, spo2);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);
  
  if (status == 200) {
    Serial.println("ThingSpeak upload OK");
    Serial.printf("ECG: %d | Temp: %.1f°C | BPM: %d | SpO2: %.1f%%\n",
                 ecgValue, objectTempC, beatAvg, spo2);
  } else {
    Serial.println("Upload failed: " + String(status));
  }
}

void loop() {
  // Read sensor data
  long irValue = max30102.getIR();
  long redValue = max30102.getRed();
  
  checkHeartRate(irValue);
  calculateSpO2(redValue, irValue);

  // Process Arduino data
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();
    if (data.length() > 0) {
      parseSensorData(data);
      
      // Check temperature threshold (100°F = 37.78°C)
      float objectTempF = (objectTempC * 9.0/5.0) + 32;
      if (objectTempF >= 100.0 && !emailSent) {
        sendEmailAlert();
      } else if (objectTempF < 100.0) {
        emailSent = false;
      }
    }
  }

  // Upload every 20 seconds
  if (millis() - lastUpload > 20000) {
    uploadToThingSpeak();
    lastUpload = millis();
  }

  delay(10);
}
