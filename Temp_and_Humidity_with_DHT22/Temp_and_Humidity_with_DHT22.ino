/*
 *     Maker      : DIYSensors                
 *     Project    : WiFi - MQTT and OTA - Basic
 *     Version    : 1.0
 *     Date       : 10-2024
 *     Programmer : Ap Matteman
 *     
 */    


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "arduino_secrets.h"

int iWiFiTry = 0;          
int iMQTTTry = 0;
String sClient_id;

//DHT22 settings
#define DHTPIN D7     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);

unsigned long lPmillis = 0;        // will store last time MQTT has published
const long lInterval = 5000; // 5000 ms => 5 Seconds for demo. You can change it to 5 minutes

const char* ssid = YourSSID;
const char* password = YourWiFiPassWord;
const char* HostName = "Office_Temp";  // make this unique!


const char* mqtt_broker = YourMQTTserver;
const char* mqtt_user = YourMQTTuser;
const char* mqtt_password = YourMQTTpassword;

WiFiClient espClient;
PubSubClient MQTTclient(espClient); // MQTT Client

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }  // wait for serial port to connect. Needed for native USB port only    
  dht.begin();       
  Connect2WiFi();
  Connect2MQTT();
}

void Connect2WiFi() { 
  //Connect to WiFi
  // WiFi.mode(WIFI_STA);  //in case of an ESP32
  iWiFiTry = 0;
  WiFi.begin(ssid, password);
  WiFi.setHostname(HostName);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED && iWiFiTry < 11) { //Try to connect to WiFi for 11 times
    ++iWiFiTry;
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  // ArduinoOTA.setPort(8266); // Port defaults to 8266
  
  ArduinoOTA.setHostname(HostName); 
  // ArduinoOTA.setPassword((const char *)OTAPassword); // You can set the password for OTA

  ArduinoOTA.onStart([]() { Serial.println("Start"); });
  ArduinoOTA.onEnd([]()   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");

  //Unique MQTT Device name
  sClient_id = "esp-client-" + String(WiFi.macAddress());
  Serial.print("ESP Client name: "); Serial.println(sClient_id);
}

void Connect2MQTT() {
  // Connect to the MQTT server
  iMQTTTry=0;
  if (WiFi.status() != WL_CONNECTED) { 
    Connect2WiFi; 
  }

  Serial.print("Connecting to MQTT ");
  MQTTclient.setServer(mqtt_broker, 1883);
  while (!MQTTclient.connect(sClient_id.c_str(), mqtt_user, mqtt_password) && iMQTTTry < 11) { //Try to connect to MQTT for 11 times
    ++iMQTTTry;
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
}

void loop() {
  ArduinoOTA.handle();
  unsigned long lCmillis = millis();

  if (lCmillis - lPmillis >= lInterval) {
    lPmillis = lCmillis;
    if (!MQTTclient.connect(sClient_id.c_str(), mqtt_user, mqtt_password)) {
      Connect2MQTT();
    }

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature!");
      MQTTclient.publish("Office/Sensor/TemperatureC", String("-180").c_str());
      MQTTclient.publish("Office/Sensor/TemperatureF", String("-180").c_str());
    }
    else {
      Serial.print("Temperature: "); Serial.print(event.temperature); Serial.println("°C");
      MQTTclient.publish("Office/Sensor/TemperatureC", String(event.temperature).c_str());
      MQTTclient.publish("Office/Sensor/TemperatureF", String((event.temperature * 1.8) + 22).c_str());
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Error reading humidity!");
      MQTTclient.publish("Office/Sensor/Humidity", String(-180).c_str());
    }
    else {
      Serial.print("Humidity: "); Serial.print(event.relative_humidity); Serial.println("%");
      MQTTclient.publish("Office/Sensor/Humidity", String(event.relative_humidity).c_str());
    }
    Serial.println("----------------------------");

    // If you want to reboot your device when WiFi is not working
    if (iWiFiTry > 10){
      Serial.println("REBOOTING");
      Serial.println(" Reboot in 2 seconds");
      Serial.println(""); 
      delay(2000);
      ESP.restart(); 
    }
  }
  
}