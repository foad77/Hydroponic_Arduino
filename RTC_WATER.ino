//sensor water
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
///////////////
//RTC
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
///////////////
//Timer
const int pumpPin = 3;
const int lightPin = 2;

int Pump_off= 330;     // pump off in minutes
int Pump_on= 30;       // pump on in minutes

int light_off= 18;      // light off in hour
int light_on= 6;       // light on in hour

int light_State = LOW;
int pump_State = LOW;

unsigned long previousMillis_pump = 0;
unsigned long previousMillis_light = 0;

long interval_pump = 0;
long interval_light = 16;
//////////////
//Fan & pump
const int heaterPin = 4;
const int fanPin = 5;
 //int t= 21;
int T_max_fan= 27;
int T_min_fan= 24;
int T_max_heater= 20;
int T_min_heater= 17;
//// WAter & humidity
#include "DHT.h"
#define DHTPIN 6     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE);
//////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running");
  }

  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 
  Serial.println("Setup complete.");
  //////////////////////
  Serial.begin(9600);
  sensors.begin();
  /////////////////////
  //timer
  pinMode(pumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  //////////////
  ///fan & pump
  pinMode(heaterPin, OUTPUT);
pinMode(fanPin, OUTPUT);
///// WATER & humidity
Serial.println("DHT11 test!"); 
  dht.begin();
}
////////////////////////////////////////////////////////////////
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
   sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print("Temperature: ");
  Serial.println(sensors.getTempCByIndex(0)); 
  //delay(1000); 
  //////////////////////////
   DateTime now = RTC.now();   
 
  // Display the current time
  Serial.print("Current time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
 
 /// delay(1000);
 /////////////////////////
 // timer
  
  unsigned long currentMillis_light = now.hour();
  if (currentMillis_light - previousMillis_light >= interval_light) {
    // save the last time you blinked the LED
      previousMillis_light = currentMillis_light;
 
    // if the LED is off turn it on and vice-versa:
    if (light_State == LOW) {
      light_State = HIGH;
      interval_light = light_off;
    } else {
      light_State = LOW;
      interval_light = light_on;
    }
  
    // set the LED with the pump_State of the variable:
    digitalWrite(lightPin, light_State);
  }
  //////////////
  unsigned long currentMillis_pump = now.minute();

   if (currentMillis_pump - previousMillis_pump >= interval_pump) {
    // save the last time you blinked the LED
      previousMillis_pump = currentMillis_pump;
      
    // if the LED is off turn it on and vice-versa:
    if (pump_State == LOW) {
      pump_State = HIGH;
      interval_pump = Pump_off;
    } else {
      pump_State = LOW;
      interval_pump = Pump_on;
    }
  
    // set the LED with the pump_State of the variable:
    digitalWrite(pumpPin, pump_State);
  }
  //// Fan & pump
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } 
  else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    
  }

  if (t >= T_max_fan){
    digitalWrite(fanPin, LOW);
    digitalWrite(heaterPin, HIGH);
  }
    else { 
       if (t <= T_min_fan){
        digitalWrite(fanPin, HIGH);
       }        
       }
   if ( t <= T_min_heater){
            digitalWrite(heaterPin, LOW);
            digitalWrite(fanPin, HIGH);
          }
         else { 
         if (t >= T_max_heater){
              digitalWrite(heaterPin, HIGH);
         }
              }
    
    
}

