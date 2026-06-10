// DHT22 Sensor with ESP32
// Connections:
// DHT22 DATA  -> D4 on ESP32
// DHT22 VCC   -> 3V3 on ESP32
// DHT22 GND   -> GND on ESP32

#include <DHT.h>

// Define the pin connected to DATA
#define DHTPIN 4        // D4 = GPIO4 on ESP32

// Define sensor type
#define DHTTYPE DHT22

// Create DHT object
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Start the DHT sensor
  dht.begin();

  Serial.println("DHT22 Sensor Reading...");
}

void loop() {

  // Read humidity
  float humidity = dht.readHumidity();

  // Read temperature in Celsius
  float temperatureC = dht.readTemperature();

  // Read temperature in Fahrenheit
  float temperatureF = dht.readTemperature(true);

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT22 sensor!");
    delay(2000);
    return;
  }

  // Print values to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" °F");

  Serial.println("------------------------");

  // Wait 2 seconds before next reading
  delay(2000);
}