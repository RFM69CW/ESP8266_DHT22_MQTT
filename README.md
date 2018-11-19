# ESP8266_DHT22_MQTT
Cheap sensor node.

## Hardware
* NodeMCU mini
* DHT 22
* 10 k resistor

## Feature

The sensor reads the temperature and humidity and publish the result to MQTT topics.

## Homekit

To integrate the results to HomeKit the HomeBridge with this Plugin (https://github.com/arachnetech/homebridge-mqttthing#readme) is used.

Example configuration:
``` 
"accessories": [
    {
        "accessory": "mqttthing",
        "type": "humiditySensor",
        "name": "Badezimmer Hum",
        "url": "http://xxx.xxx.xxx.xxx:1883",
        "logMqtt": true,
        "topics": {
            "getCurrentRelativeHumidity": "bath/hum",
            "getStatusActive": "bath/sensor/active"
        },
        "integerValue": true
    },
    {
        "accessory": "mqttthing",
        "type": "temperatureSensor",
        "name": "Badezimmer Temp",
        "url": "http://xxx.xxx.xxx.xxx:1883",
        "logMqtt": true,
        "topics": {
            "getCurrentTemperature": "bath/temp",
            "getStatusActive": "bath/sensor/active"
        },
        "integerValue": true
    }
]

```

