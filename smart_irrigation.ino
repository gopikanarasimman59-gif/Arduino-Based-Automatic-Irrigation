
/*
  Project: Smart Irrigation System
  Board: Arduino Uno
  Sensor: Soil Moisture Sensor (Analog)
  Output: Relay Module + DC Water Pump

  Connections:
  Soil Sensor AO -> A0
  Relay IN       -> D7
  Sensor VCC     -> 5V (if supported)
  Sensor GND     -> GND
*/

// Pin configuration
const int soilSensorPin = A0;
const int relayPin = 7;

// Calibrate these values for your sensor
const int dryValue = 800;
const int wetValue = 350;

// Moisture thresholds
const int pumpOnThreshold = 35;
const int pumpOffThreshold = 45;

// Many relay modules are active LOW
const bool relayActiveLow = true;

bool pumpStatus = false;

void controlPump(bool turnOn) {
  pumpStatus = turnOn;

  if (relayActiveLow) {
    digitalWrite(relayPin, turnOn ? LOW : HIGH);
  } else {
    digitalWrite(relayPin, turnOn ? HIGH : LOW);
  }
}

int readSoilMoisture() {
  int rawValue = analogRead(soilSensorPin);

  int moisture = map(
    rawValue, dryValue, wetValue, 0, 100
  );

  return constrain(moisture, 0, 100);
}

void setup() {
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  controlPump(false);

  Serial.println("Smart Irrigation System Started");
  Serial.println("--------------------------------");
}

void loop() {
  int rawValue = analogRead(soilSensorPin);
  int moisture = readSoilMoisture();

  // Automatic pump control with two thresholds
  if (!pumpStatus && moisture < pumpOnThreshold) {
    controlPump(true);
  }
  else if (pumpStatus && moisture >= pumpOffThreshold) {
    controlPump(false);
  }

  // Display readings
  Serial.print("Raw Sensor Value: ");
  Serial.print(rawValue);

  Serial.print(" | Soil Moisture: ");
  Serial.print(moisture);
  Serial.print("%");

  Serial.print(" | Water Pump: ");
  Serial.println(pumpStatus ? "ON" : "OFF");

  delay(2000);
}
