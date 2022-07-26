#ifndef WiFiHandler_h
#define WiFiHandler_h

#include <HardwareSerial.h>
#include "../Logger/Logger.h"
#include <WiFiManager.h>

void setup_wifi(const char* AP_SSID, const char* password);

void setup_ota(const char* hostname, const char* password);

void handle_ota();

#endif