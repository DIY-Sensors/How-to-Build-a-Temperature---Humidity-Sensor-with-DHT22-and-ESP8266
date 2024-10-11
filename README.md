

# DIY Temperature & Humidity Sensor with DHT22 and ESP8266 for HomeAssistant
This project demonstrates how to build a temperature and humidity sensor using the DHT22 sensor and the ESP8266 (Wemos D1 Mini) board, perfect for integrating into your Home Assistant setup. The sensor communicates over WiFi using MQTT, and this repository contains both Arduino and ESPHome code to get you up and running.

## Components Needed:
- DHT22 Sensor
- ESP8266 (Wemos D1 Mini or NodeMCU)
- Jumper wires
- Breadboard
- Home Assistant (with MQTT broker installed)

## Features:
- Accurate temperature (in Celcius and Farenheid)and humidity readings with DHT22
- WiFi connectivity via ESP8266
- MQTT communication to Home Assistant
- Code available for both Arduino and ESPHome

## Getting Started:
1. Clone or download this repository.
2. Choose between the Arduino or ESPHome code depending on your preference.
### Arduino Setup:
- Open the Arduino code in the Arduino IDE.
- Install the required libraries:
  - ESP8266 Board library
  - DHT Sensor library
- Edit the code to add your WiFi and MQTT credentials.
- Upload the code to your ESP8266.

## ESPHome Setup:
- Open the provided YAML file in your ESPHome configuration.
- Add your WiFi and MQTT credentials.
- Upload the configuration to your ESP8266.

## Video Tutorial:
For a detailed guide on how to build and set up the sensor, check out the full YouTube video here:
How to Build a Temperature & Humidity Sensor with DHT22 and ESP8266 | DIY Home Automation Project

## License:
This project is open-sourced under the MIT License. See the LICENSE file for more details.
