#include <WiFi.h>
#include <HTTPClient.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// WIFI
const char* ssid = "Dialog 4G 082";
const char* password = "Fbfm72nH";

// Flask server
String serverURL = "http://192.168.8.103:5000/capture";

// Pump pins
#define PUMP_N 26
#define PUMP_K 14
#define PUMP_P 13
#define PUMP_W 2

void setup() {

  Serial.begin(115200);
  SerialBT.begin("AgriRobot");

  pinMode(PUMP_N, OUTPUT);
  pinMode(PUMP_K, OUTPUT);
  pinMode(PUMP_P, OUTPUT);
  pinMode(PUMP_W, OUTPUT);

  digitalWrite(PUMP_N, LOW);
  digitalWrite(PUMP_K, LOW);
  digitalWrite(PUMP_P, LOW);
  digitalWrite(PUMP_W, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");
}

void loop() {

  if (SerialBT.available()) {

    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    // Existing movement commands
    if (cmd == "F") {
      // forward
    }

    if (cmd == "B") {
      // backward
    }

    // AI Capture command
    if (cmd == "C") {

      Serial.println("Capturing image...");

      HTTPClient http;

      http.begin(serverURL);

      int httpCode = http.GET();

      if (httpCode > 0) {

        String payload = http.getString();

        Serial.println(payload);

        if (payload.indexOf("Nitrogen") >= 0) {
          sprayPump(PUMP_N);
        }

        else if (payload.indexOf("Potassium") >= 0) {
          sprayPump(PUMP_K);
        }

        else if (payload.indexOf("Phosphorus") >= 0) {
          sprayPump(PUMP_P);
        }

        else if (payload.indexOf("Water_Stress") >= 0) {
          sprayPump(PUMP_W);
        }

        else {
          Serial.println("Healthy plant");
        }

      } else {
        Serial.println("Server Error");
      }

      http.end();
    }
  }
}

void sprayPump(int pin) {

  digitalWrite(pin, HIGH);

  delay(5000);

  digitalWrite(pin, LOW);

  Serial.println("Spraying done");
}