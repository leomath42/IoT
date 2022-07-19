#ifndef WiFiHandler_h
#define WiFiHandler_h

#include <HardwareSerial.h>
#include "../Logger/Logger.h"
#include <WiFiManager.h>

void reconectWiFi();

void setup_wifi();

void setup_ota();

void handle_ota();

void setup_MQTT();

void mqtt_callback(char* topic, byte* payload, unsigned int length);

void reconnectMQTT();

void checkConnectionWiFiMQTT();

#endif