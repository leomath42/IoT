#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"


#define TIME_TO_WAIT 60000

long last_read = 0;
long last_time = 0;

char buffer[32];

void setup()
{
    buffer[30] = '\n';
    buffer[31] = 0;

    Serial.begin(115200);
    
    setup_weather();
    
    display_setup();
    
    setup_wifi();
}

void loop()
{
    last_time = millis();

    if(10000 < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f  Temp: %.1f", weather.humidity, weather.temperature);
        logger_print(buffer);
    }

// OTA, MQTT
// 
// WiFiManager

// Interrupção hardware -> push button 
// time 
// watchdog

}
