# Hydroponic Climate and Water Pump Control System

## Overview

This project is designed to control a hydroponic climate system, including temperature, humidity, light, and water pump regulation, using an Arduino Uno. The system monitors environmental parameters and adjusts the climate and water flow accordingly to ensure optimal growth conditions for plants in a hydroponic setup.

### System Logic

1. **Temperature and Humidity Control:**
   - The system uses a DHT11 sensor to monitor the temperature and humidity levels.
   - Based on the temperature readings, the system controls a fan and a heater:
     - If the temperature exceeds a certain threshold (`T_max_fan`), the fan is turned on to cool down the environment.
     - If the temperature drops below a certain threshold (`T_min_heater`), the heater is turned on to warm up the environment.

2. **Light Control:**
   - The system controls the light based on the time of day, using a Real-Time Clock (RTC) module to keep track of the current time.
   - The lights turn on during the specified hours (e.g., from 6 AM to 6 PM) and turn off during the night (e.g., from 6 PM to 6 AM).

3. **Water Pump Control:**
   - The water pump operates on a timed cycle, turning on for a specified duration (e.g., 30 minutes) and then turning off for a longer duration (e.g., 330 minutes).
   - This regular on/off cycle ensures that the plants receive adequate water without over-saturating the growing medium.

### Input/Output Ports of the Arduino Uno

- **Digital Pins:**
  - **Pin 2:** Controls the light (output).
  - **Pin 3:** Controls the water pump (output).
  - **Pin 4:** Controls the heater (output).
  - **Pin 5:** Controls the fan (output).
  - **Pin 6:** Reads data from the DHT11 temperature and humidity sensor (input).
  - **Pin 7:** Communicates with the DS18B20 temperature sensor via the One-Wire protocol (input/output).

- **I2C Pins:**
  - **SDA (A4) and SCL (A5):** Communicate with the RTC module (RTC_DS1307) using the I2C protocol.

### Serial Communication Protocols Used

This project involves multiple serial communication protocols to interface with different sensors and modules.

1. **I2C Protocol (Inter-Integrated Circuit):**
   - **Used For:** Communicating with the RTC module (RTC_DS1307).
   - **How It Works:** The I2C protocol uses two wires (SDA for data and SCL for the clock) to communicate between the Arduino and the RTC module. This protocol allows the Arduino to read the current time and adjust the light control based on the time of day.

2. **One-Wire Protocol:**
   - **Used For:** Communicating with the DS18B20 temperature sensor.
   - **How It Works:** The One-Wire protocol allows multiple devices to communicate over a single data line. In this project, the DS18B20 temperature sensor uses the One-Wire protocol to send temperature data to the Arduino. This data is used to regulate the climate (fan and heater control).

3. **UART Protocol (Universal Asynchronous Receiver-Transmitter):**
   - **Used For:** Debugging via the Serial Monitor in the Arduino IDE.
   - **How It Works:** The UART protocol is used for serial communication between the Arduino and the computer. In this project, it's used to print debug messages and sensor readings to the Serial Monitor, helping to monitor the system's performance and troubleshoot issues.

## Getting Started

To use this code, you will need the following libraries installed in your Arduino IDE:

- `OneWire`
- `DallasTemperature`
- `Wire`
- `RTClib`
- `DHT`

### Hardware Requirements

- Arduino Uno
- DS18B20 Temperature Sensor
- DHT11 Temperature and Humidity Sensor
- RTC Module (RTC_DS1307)
- Relay Modules for controlling the pump, light, fan, and heater
- Appropriate power supply and wiring

### Installation

1. Connect the sensors and modules to the Arduino Uno as described in the input/output section.
2. Upload the code to the Arduino using the Arduino IDE.
3. Open the Serial Monitor to observe debug messages and sensor readings.

### Usage

Once the code is uploaded and running, the system will automatically control the climate and water pump based on the sensor readings and time settings.

- **Light Control:** Automatically adjusts based on the RTC time.
- **Pump Control:** Operates on a timed cycle.
- **Fan and Heater Control:** Adjusts based on temperature readings.

## Future Improvements

- Add a user interface to manually control the system and adjust parameters.
- Integrate additional sensors for more accurate climate control.
- Implement data logging to track system performance over time.

## License

This project is open-source and available under the MIT License. Feel free to modify and use it for your hydroponic system!

