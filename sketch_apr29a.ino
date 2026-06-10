#include <BluetoothSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

BluetoothSerial SerialBT;

// WiFi
const char* ssid      = "Dialog 4G 082";
const char* password  = "Fbfm72nH";
const char* serverURL = "http://192.168.8.101:5000/capture";

// Motors
#define LM_IN1 26
#define LM_IN2 27
#define RM_IN1 14
#define RM_IN2 12

// Pumps
#define N_IN1 25
#define N_IN2 33
#define K_IN1 32
#define K_IN2 34
#define P_IN1 13
#define P_IN2 15
#define W_IN1 4
#define W_IN2 16

void setup() {
  Serial.begin(115200);
  SerialBT.begin("AgriRobot");

  const uint8_t pins[] = {LM_IN1, LM_IN2, RM_IN1, RM_IN2, N_IN1, N_IN2, K_IN1, K_IN2, P_IN1, P_IN2, W_IN1, W_IN2};
  for (uint8_t p : pins) { pinMode(p, OUTPUT); digitalWrite(p, LOW); }

  WiFi.begin(ssid, password);
  Serial.print("WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("OK:" + WiFi.localIP().toString());
  SerialBT.println("Ready. F/B/L/R/S | N/K/P/W | C");
}

void loop() {
  if (!SerialBT.available()) return;

  String cmd = SerialBT.readStringUntil('\n');
  cmd.trim();
  if (cmd.length() == 0) return;

  char c = toupper(cmd[0]);
  Serial.println(cmd);

  switch (c) {
    case 'F': drive(HIGH, LOW, HIGH, LOW, "Fwd"); break;
    case 'B': drive(LOW, HIGH, LOW, HIGH, "Bck"); break;
    case 'L': drive(LOW, LOW, HIGH, LOW,  "Lft"); break;
    case 'R': drive(HIGH, LOW, LOW, LOW,  "Rgt"); break;
    case 'S': stopCar(); break;
    case '1': spray(N_IN1, N_IN2, "N"); break;
    case '2': spray(K_IN1, K_IN2, "K"); break;
    case '3': spray(P_IN1, P_IN2, "P"); break;
    case '4': spray(W_IN1, W_IN2, "W"); break;
    case 'Y': aiScan(); break;
    default:  SerialBT.println("?"); break;
  }
}

void drive(uint8_t l1, uint8_t l2, uint8_t r1, uint8_t r2, const char* name) {
  digitalWrite(LM_IN1, l1); digitalWrite(LM_IN2, l2);
  digitalWrite(RM_IN1, r1); digitalWrite(RM_IN2, r2);
  SerialBT.println(name);
}

void stopCar() {
  digitalWrite(LM_IN1, LOW); digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN1, LOW); digitalWrite(RM_IN2, LOW);
  SerialBT.println("Stopped");
}

void spray(uint8_t in1, uint8_t in2, const char* label) {
  SerialBT.print(label); SerialBT.println(" ON");
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  delay(4000);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  SerialBT.println("Done");
}

void aiScan() {
  if (WiFi.status() != WL_CONNECTED) {
    SerialBT.println("No WiFi");
    return;
  }

  HTTPClient http;
  http.begin(serverURL);
  http.setTimeout(10000);

  int code = http.GET();
  if (code <= 0) {
    SerialBT.println("Server err");
    http.end();
    return;
  }

  String resp = http.getString();
  http.end();

  SerialBT.println(resp);  // print full response for debugging

  // Extract the "pump" field value from JSON e.g. "pump":"Nitrogen"
  String pump = "";
  int pumpIdx = resp.indexOf("\"pump\":\"");
  if (pumpIdx >= 0) {
    int start = pumpIdx + 8;  // skip past "pump":"
    int end   = resp.indexOf("\"", start);
    pump      = resp.substring(start, end);
  }

  pump.toLowerCase();
  SerialBT.println("Pump: " + pump);

  if      (pump == "nitrogen")   spray(N_IN1, N_IN2, "N");
  else if (pump == "potassium")  spray(K_IN1, K_IN2, "K");
  else if (pump == "phosphorus") spray(P_IN1, P_IN2, "P");
  else if (pump == "water")      spray(W_IN1, W_IN2, "W");
  else if (pump == "none")       SerialBT.println("Healthy - no spray");
  else                           SerialBT.println("Unknown: " + pump);
}