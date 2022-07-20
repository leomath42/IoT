#include "WiFiHandler.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>


WiFiManager wifiManager;
ESP8266WiFiClass wifi;

void setup_wifi()
{
    // wifiManager.resetSettings();
    wifiManager.setConnectTimeout(60);

    logger_print("Starting WiFi...");
    delay(1000);
    if(wifi.status() == WL_DISCONNECTED)
    {
        logger_print("Failed to connect.");
        delay(1000);
        logger_print("Going to init AP mode.");
        delay(1000);
        logger_print("AP: ESP");
        wifiManager.autoConnect("ESP", "12345678");
    }
    else{
      Serial.println("IP address: " +  wifi.localIP().toString());
      delay(1000);
    }

}

void handle_ota()
{
    ArduinoOTA.handle();
}

void setup_ota()
{
  Serial.println("Iniciando OTA....");
  ArduinoOTA.setHostname("pratica-4"); // Define o nome da porta

  // No authentication by default
   ArduinoOTA.setPassword((const char *)"teste-ota"); // senha para carga via WiFi (OTA)
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
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
}

