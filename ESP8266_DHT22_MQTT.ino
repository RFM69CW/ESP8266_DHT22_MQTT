// Required libraries:
//  - Adafruit Unified Sensor (required for DHT)
//  - DHT Sensor Library
//  - PubSubClient (for MQTT)
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>

#define SENSORLOCATION "bedroom"
//#define SENSORLOCATION "bath"

#define MQTT_HOST "192.168.xxx.xxx"
#define MQTT_PORT 1883
#define MQTT_CLIENT "ESP8266_dht22_" SENSORLOCATION
#define MQTT_TOPIC_H1 SENSORLOCATION "/hum" 
#define MQTT_TOPIC_T1 SENSORLOCATION "/temp"
#define MQTT_TOPIC_ACTIVE SENSORLOCATION "/sensor/active"

#define WIFI_SSID SENSORLOCATION "_Sensor"
#define WIFI_PASS "admin"
#define RESET_PIN 14 //D5 on nodeMCU

WiFiClient espClient;
PubSubClient client(espClient); 

#define REPORT_DELAY_SECS 30

DHT dht22(D3, DHT22); //DHT22 wired to D3

void setup() {
  WiFiManager wifiManager;
  Serial.begin(115200);


  setupResetHandling(wifiManager, RESET_PIN);
 
  connectToWifi(wifiManager);
  
  client.setServer(MQTT_HOST, MQTT_PORT); 
  
  dht22.begin();
}

void loop() {

  if (!client.connected()) {
    mqttConnect();
  }
  client.loop(); 
  readAndPublishDHT22();
  
  delay(REPORT_DELAY_SECS * 1000);
}
void mqttConnect() {
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    if (client.connect(MQTT_CLIENT)) {
      Serial.println(F("connected"));
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      delay(5000);
    }
  }
}

void readAndPublishDHT22() {
  float h1 = dht22.readHumidity();
  float t1 = dht22.readTemperature();
  if (isnan(h1) || isnan(t1)) {
    Serial.println(F("Failed to read from DHT22 sensor!"));
    sendActiveState(false);
    return;
  }
  Serial.println("DHT22 Humidity:    " + String(h1));
  Serial.println("DHT22 Temperature: " + String(t1));

  client.publish(MQTT_TOPIC_H1, String(h1).c_str(), true);
  client.publish(MQTT_TOPIC_T1, String(t1).c_str(), true); 
  sendActiveState(true);

  Serial.println(F("------------------------"));
}

void sendActiveState(boolean isActive) {
  client.publish(MQTT_TOPIC_ACTIVE, String(isActive).c_str(), true); 
}

