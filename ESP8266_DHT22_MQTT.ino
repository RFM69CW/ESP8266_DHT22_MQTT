// Required libraries:
//  - Adafruit Unified Sensor (required for DHT)
//  - DHT Sensor Library
//  - PubSubClient (for MQTT)

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#define MQTT_HOST "*"
#define MQTT_PORT 1883
#define MQTT_USER "*"
#define MQTT_PASS "*"
#define MQTT_CLIENT "ESP8266_living_dht22"
#define MQTT_TOPIC_H1 "living/temp" 
#define MQTT_TOPIC_T1 "living/hum"

#define WIFI_SSID "*"
#define WIFI_PASS "*"

WiFiClient espClient;
PubSubClient client(espClient); 

#define REPORT_DELAY_SECS 30

DHT dht22(D3, DHT22); //DHT22 wired to D3

void setup() {
  
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA); 

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }


  client.setServer(MQTT_HOST, MQTT_PORT); 
  
  dht22.begin();
}

void loop() {

  delay(REPORT_DELAY_SECS * 1000);

  if (!client.connected()) {
    mqttConnect();
  }
  client.loop(); 
  readAndPublishDHT22();
  
}

void log(char * msg, float value) {
  if (value != NULL) {
    Serial.print(value);
  }
  Serial.println();
}

void mqttConnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void readAndPublishDHT22() {
  float h1 = dht22.readHumidity();
  float t1 = dht22.readTemperature();
  if (isnan(h1) || isnan(t1)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }
  log("DHT22 Humidity:    ", h1);
  log("DHT22 Temperature: ", t1);

  client.publish(MQTT_TOPIC_H1, String(h1).c_str(), true);
  client.publish(MQTT_TOPIC_T1, String(t1).c_str(), true); 

  log("------------------------", NULL);
}

