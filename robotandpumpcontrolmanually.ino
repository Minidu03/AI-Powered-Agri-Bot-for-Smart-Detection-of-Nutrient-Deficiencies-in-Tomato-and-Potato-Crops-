#include <BluetoothSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

BluetoothSerial SerialBT;

// ---------------- WIFI (for AI part) ----------------
const char* ssid = "Dialog 4G 082";
const char* password = "Fbfm72nH";
String serverURL = "http://192.168.8.103:5000/capture";

// ---------------- CAR MOTOR PINS ----------------
// Left motor
#define LM_IN1 26
#define LM_IN2 27

// Right motor
#define RM_IN1 14
#define RM_IN2 12

// ---------------- PUMP PINS ----------------
#define N_IN1 25
#define N_IN2 33

#define K_IN1 32
#define K_IN2 35

#define P_IN1 13
#define P_IN2 15

#define W_IN1 4
#define W_IN2 16

void setup() {

  Serial.begin(115200);
  SerialBT.begin("AgriRobot");

  // Motor pins
  pinMode(LM_IN1, OUTPUT); pinMode(LM_IN2, OUTPUT);
  pinMode(RM_IN1, OUTPUT); pinMode(RM_IN2, OUTPUT);

  pinMode(N_IN1, OUTPUT); pinMode(N_IN2, OUTPUT);
  pinMode(K_IN1, OUTPUT); pinMode(K_IN2, OUTPUT);
  pinMode(P_IN1, OUTPUT); pinMode(P_IN2, OUTPUT);
  pinMode(W_IN1, OUTPUT); pinMode(W_IN2, OUTPUT);

  stopCar();
  stopPumps();

  // WiFi for ML server
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop() {

  if (SerialBT.available()) {

    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    Serial.println("CMD: " + cmd);

    // ---------------- CAR CONTROL ----------------
    if (cmd == "F") forward();
    else if (cmd == "B") backward();
    else if (cmd == "L") left();
    else if (cmd == "R") right();
    else if (cmd == "S") stopCar();

    // ---------------- PUMP CONTROL MANUAL ----------------
    else if (cmd == "1") sprayPump(N_IN1, N_IN2);
    else if (cmd == "2") sprayPump(K_IN1, K_IN2);
    else if (cmd == "3") sprayPump(P_IN1, P_IN2);
    else if (cmd == "4") sprayPump(W_IN1, W_IN2);

    // ---------------- AI MODE ----------------
    else if (cmd == "C") {

      Serial.println("AI Capture...");

      HTTPClient http;
      http.begin(serverURL);

      int code = http.GET();

      if (code > 0) {

        String payload = http.getString();
        payload.trim();
        payload.toLowerCase();

        Serial.println("AI: " + payload);

        if (payload.indexOf("nitrogen") >= 0) sprayPump(N_IN1, N_IN2);
        else if (payload.indexOf("potassium") >= 0) sprayPump(K_IN1, K_IN2);
        else if (payload.indexOf("phosphorus") >= 0) sprayPump(P_IN1, P_IN2);
        else if (payload.indexOf("water") >= 0) sprayPump(W_IN1, W_IN2);
        else Serial.println("Healthy plant");

      } else {
        Serial.println("Server error");
      }

      http.end();
    }
  }
}

// ================= CAR FUNCTIONS =================
void forward() {
  digitalWrite(LM_IN1, HIGH); digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN1, HIGH); digitalWrite(RM_IN2, LOW);
}

void backward() {
  digitalWrite(LM_IN1, LOW); digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN1, LOW); digitalWrite(RM_IN2, HIGH);
}

void left() {
  digitalWrite(LM_IN1, LOW); digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN1, HIGH); digitalWrite(RM_IN2, LOW);
}

void right() {
  digitalWrite(LM_IN1, HIGH); digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN1, LOW); digitalWrite(RM_IN2, LOW);
}

void stopCar() {
  digitalWrite(LM_IN1, LOW); digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN1, LOW); digitalWrite(RM_IN2, LOW);
}

// ================= PUMP FUNCTIONS =================
void sprayPump(int in1, int in2) {

  Serial.println("Pump ON");

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  delay(4000);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  Serial.println("Pump OFF");
}

void stopPumps() {
  digitalWrite(N_IN1, LOW); digitalWrite(N_IN2, LOW);
  digitalWrite(K_IN1, LOW); digitalWrite(K_IN2, LOW);
  digitalWrite(P_IN1, LOW); digitalWrite(P_IN2, LOW);
  digitalWrite(W_IN1, LOW); digitalWrite(W_IN2, LOW);
}