#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// Motor pins (L298N)
#define IN1 26
#define IN2 27
#define IN3 14
#define IN4 12

// Pump / Relay pins
#define PUMP1 32
#define PUMP2 33
#define PUMP3 25
#define PUMP4 13
#define FERT1 15
#define FERT2 2
#define FERT3 4

char cmd;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("FarmRobot"); // Bluetooth name

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Pumps
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);

  pinMode(FERT1, OUTPUT);
  pinMode(FERT2, OUTPUT);
  pinMode(FERT3, OUTPUT);

  stopRobot();
}

void loop() {
  if (SerialBT.available()) {
    cmd = SerialBT.read();
    Serial.println(cmd);

    // 🚗 Movement control
    if (cmd == 'F') forward();
    else if (cmd == 'B') backward();
    else if (cmd == 'L') left();
    else if (cmd == 'R') right();
    else if (cmd == 'S') stopRobot();

    // 💧 Water pumps
    else if (cmd == '1') digitalWrite(PUMP1, HIGH);
    else if (cmd == '2') digitalWrite(PUMP1, LOW);

    else if (cmd == '3') digitalWrite(PUMP2, HIGH);
    else if (cmd == '4') digitalWrite(PUMP2, LOW);

    else if (cmd == '5') digitalWrite(PUMP3, HIGH);
    else if (cmd == '6') digitalWrite(PUMP3, LOW);

    else if (cmd == '7') digitalWrite(PUMP4, HIGH);
    else if (cmd == '8') digitalWrite(PUMP4, LOW);

    // 🌿 Fertilizer
    else if (cmd == '9') digitalWrite(FERT1, HIGH);
    else if (cmd == '0') digitalWrite(FERT1, LOW);
  }
}

// 🚗 Movement functions
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}