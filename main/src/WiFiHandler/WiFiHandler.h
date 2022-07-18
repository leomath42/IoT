#ifndef WiFiHandler_h
#define WiFiHandler_h

#include <HardwareSerial.h>
#include "../Logger/Logger.h"
#include <WiFiManager.h>

void setup_wifi();

void setup_ota();

void handle_ota();

#endif