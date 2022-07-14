#include "WiFiHandler.h"


WiFiManager wifiManager;
ESP8266WiFiClass wifi;

void setup_wifi()
{
    // wifiManager.resetSettings();
    wifiManager.setConnectTimeout(10);
    Serial.println("AAAAAAAAAA");
    logger_print("Starting WiFi...");

    if(wifi.status() == WL_DISCONNECTED)
    {
        Serial.println("BBBBBBBBBBBBBB");
        logger_print("Failed to connect.");
        delay(1000);
        logger_print("Going to init AP mode.");
        delay(1000);
        logger_print("AP: ESP");
        wifiManager.autoConnect("ESP", "12345678");
        Serial.println("CCCCCCCCCCCCC");
    }

}

