// Include necessary libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"

// Pin Definitions
#define ONE_WIRE_BUS 7     // Pin for DS18B20 temperature sensor
#define DHTPIN 6           // Pin for DHT11 sensor
#define DHTTYPE DHT11      // DHT11 sensor type

// Device Pins
const int pumpPin = 3;     // Pin for Pump control
const int lightPin = 2;    // Pin for Light control
const int heaterPin = 4;   // Pin for Heater control
const int fanPin = 5;      // Pin for Fan control

// Timing Variables for Pump and Light Control
const int pumpOffMinutes = 330;  // Pump off duration in minutes
const int pumpOnMinutes = 30;    // Pump on duration in minutes
const int lightOffHour = 18;     // Light off hour (24-hour format)
const int lightOnHour = 6;       // Light on hour (24-hour format)

// Temperature Control Variables
const int T_max_fan = 27;        // Maximum temperature to turn on Fan (in °C)
const int T_min_fan = 24;        // Minimum temperature to turn off Fan (in °C)
const int T_max_heater = 20;     // Maximum temperature to turn off Heater (in °C)
const int T_min_heater = 17;     // Minimum temperature to turn on Heater (in °C)

// State Variables
int lightState = LOW;           // Current state of the Light
int pumpState = LOW;            // Current state of the Pump

// Timing Control Variables
unsigned long previousMillisPump = 0;   // Stores last time Pump state was updated
unsigned long previousMillisLight = 0;  // Stores last time Light state was updated

// Initialize libraries
OneWire oneWire(ONE_WIRE_BUS);           // Setup OneWire instance for DS18B20
DallasTemperature sensors(&oneWire);     // Setup DallasTemperature instance
RTC_DS1307 rtc;                          // Setup RTC instance
DHT dht(DHTPIN, DHTTYPE);                // Setup DHT11 instance

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC failed to initialize");
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Update RTC with compile time if needed
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialize Sensors
  sensors.begin();
  dht.begin();

  // Initialize Device Pins
  pinMode(pumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  Serial.println("Setup complete.");
}

void loop() {
  // Request temperature data from DS18B20 sensor
  sensors.requestTemperatures();

  // Get the current time
  DateTime now = rtc.now();

  // Control Light based on time of day
  controlLight(now);

  // Control Pump based on time intervals
  controlPump(now);

  // Control Fan and Heater based on temperature
  controlFanAndHeater();
}

// Function to control light based on the time of day
void controlLight(DateTime now) {
  if (now.hour() >= lightOnHour && now.hour() < lightOffHour) {
    lightState = HIGH;
  } else {
    lightState = LOW;
  }
  digitalWrite(lightPin, lightState);
}

// Function to control pump based on time intervals
void controlPump(DateTime now) {
  unsigned long currentMillis = millis();
  unsigned long intervalPump = (pumpState == HIGH) ? pumpOnMinutes * 60000 : pumpOffMinutes * 60000;

  if (currentMillis - previousMillisPump >= intervalPump) {
    pumpState = (pumpState == HIGH) ? LOW : HIGH;  // Toggle pump state
    previousMillisPump = currentMillis;  // Update the last time the pump was toggled
  }
  digitalWrite(pumpPin, pumpState);
}

// Function to control fan and heater based on temperature
void controlFanAndHeater() {
  float temperature = dht.readTemperature();  // Read temperature from DHT11 sensor

  // Fan Control Logic
  if (temperature >= T_max_fan) {
    digitalWrite(fanPin, HIGH);  // Turn on fan
  } else if (temperature <= T_min_fan) {
    digitalWrite(fanPin, LOW);   // Turn off fan
  }

  // Heater Control Logic
  if (temperature <= T_min_heater) {
    digitalWrite(heaterPin, HIGH);  // Turn on heater
  } else if (temperature >= T_max_heater) {
    digitalWrite(heaterPin, LOW);   // Turn off heater
  }
}
