#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Wi-Fi Credentials
const char* ssid = "GyroCar";
const char* password = "12345678";

// Motor control pins
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

// Ultrasonic sensor pins
#define TRIG_PIN 25
#define ECHO_PIN 26

// Global variables
char lastCommand = 'S'; // Last received command
unsigned long lastDistanceCheck = 0; // For timing distance checks

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  
  // Start Wi-Fi AP
  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  
  // WebSocket and HTTP server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  
  stop(); // Initialize stopped
}

void loop() {
  webSocket.loop();
  server.handleClient();
  
  // Check distance every 100ms
  if (millis() - lastDistanceCheck >= 100) {
    float distance = getDistanceCM();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    if (distance <= 20.0) {
      stop();
      Serial.println("Obstacle detected, stopping");
    } else {
      if (lastCommand != 'S') {
        switch (lastCommand) {
          case 'F': forward(); break;
          case 'B': backward(); break;
          case 'L': left(); break;
          case 'R': right(); break;
        }
        Serial.print("Moving: ");
        Serial.println(lastCommand);
      } else {
        stop();
        Serial.println("No command, stopping");
      }
    }
    lastDistanceCheck = millis();
  }
}

float getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_TEXT:
      char cmd = (char)payload[0];
      Serial.print("[RECEIVED] Command: ");
      Serial.println(cmd);
      lastCommand = cmd;
      break;
  }
}

// Motor control functions
void forward() { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
void backward() { digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); }
void left() { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); }
void right() { digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); }
void stop() { digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); }

void handleRoot() {
  server.send(200, "text/html", 
    "<!DOCTYPE html>"
    "<html lang=\"en\">"
    "<head>"
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\">"
    "<title>Gyro Car Control</title>"
    "<style>"
    "* {"
    "  margin: 0;"
    "  padding: 0;"
    "  box-sizing: border-box;"
    "}"
    "body {"
    "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
    "  min-height: 100vh;"
    "  display: flex;"
    "  flex-direction: column;"
    "  justify-content: center;"
    "  align-items: center;"
    "  background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%);"
    "  padding: 20px;"
    "}"
    ".container {"
    "  width: 100%;"
    "  max-width: 500px;"
    "  background: rgba(255, 255, 255, 0.95);"
    "  padding: 2rem;"
    "  border-radius: 20px;"
    "  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);"
    "}"
    "h1 {"
    "  color: #2a5298;"
    "  margin-bottom: 2rem;"
    "  font-size: 2rem;"
    "  text-align: center;"
    "}"
    ".grid {"
    "  display: grid;"
    "  grid-template-columns: repeat(2, 1fr);"
    "  gap: 1rem;"
    "  margin-bottom: 1rem;"
    "}"
    "button {"
    "  padding: 1.5rem;"
    "  font-size: 1.1rem;"
    "  border: none;"
    "  border-radius: 12px;"
    "  color: white;"
    "  cursor: pointer;"
    "  transition: all 0.3s ease;"
    "  display: flex;"
    "  align-items: center;"
    "  justify-content: center;"
    "  gap: 0.5rem;"
    "  text-transform: uppercase;"
    "  font-weight: 600;"
    "}"
    "button:hover {"
    "  transform: translateY(-2px);"
    "  box-shadow: 0 5px 15px rgba(0, 0, 0, 0.2);"
    "}"
    "button:active {"
    "  transform: translateY(0);"
    "}"
    ".forward { background: #00b894; }"
    ".backward { background: #e17055; }"
    ".left { background: #0984e3; }"
    ".right { background: #6c5ce7; }"
    ".stop { background: #d63031; grid-column: span 2; }"
    "@media (max-width: 480px) {"
    "  .container {"
    "    padding: 1.5rem;"
    "    border-radius: 15px;"
    "  }"
    "  button {"
    "    padding: 1.2rem;"
    "    font-size: 1rem;"
    "  }"
    "  h1 {"
    "    font-size: 1.75rem;"
    "  }"
    "}"
    "</style>"
    "</head>"
    "<body>"
    "  <div class=\"container\">"
    "    <h1>🚗 Gyro Car Control</h1>"
    "    <div class=\"grid\">"
    "      <button class=\"forward\" onclick=\"sendCmd('F')\">▲ Forward</button>"
    "      <button class=\"backward\" onclick=\"sendCmd('B')\">▼ Backward</button>"
    "      <button class=\"left\" onclick=\"sendCmd('L')\">◀ Left</button>"
    "      <button class=\"right\" onclick=\"sendCmd('R')\">▶ Right</button>"
    "      <button class=\"stop\" onclick=\"sendCmd('S')\">⏹ Stop</button>"
    "    </div>"
    "  </div>"
    "<script>"
    "const ws = new WebSocket('ws://' + location.hostname + ':81/');"
    "ws.onopen = () => console.log('Connected to controller');"
    "ws.onclose = () => alert('Connection lost! Refresh page.');"
    "ws.onerror = (err) => console.error('WebSocket error:', err);"
    "function sendCmd(cmd) { ws.send(cmd); }"
    "</script>"
    "</body>"
    "</html>"
  );
}
