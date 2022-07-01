#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
// #include "teste.h"

WiFiManager wifiManager;
ESP8266WiFiClass wifi;

void setup()
{
//    setup_wifi();
//    setup_wifi();
//    setup_wifi();
//    setup_wifi();
//    setup_wifi();
   setup_wifi();
   delay(10000);
    // teste();
    Serial.begin(115200);
    wifiManager.autoConnect("ESP", "12345678");
}

void loop()
{
    setup_wifi();
    // if(wifi.isConnected())
    // {
    //     Serial.print("Connected in the AP:");
    //     Serial.println(wifi.SSID());
    //     Serial.print("hostname in the AP:");
    //     Serial.println(wifi.hostname());
    // }
	
}
