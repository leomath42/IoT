#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"


#define TIME_TO_WAIT 10000

long last_read = 0;
long last_time = 0;

char buffer[32];

WeatherStack stack;

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

    if(TIME_TO_WAIT < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        //int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f  Temp: %.1f", weather.humidity, weather.temperature);
        //logger_print(buffer);

        bool success = push_weather(&stack, weather);
        Weather* pweather = pop_weather(&stack);

        if(pweather == NULL)
        {
            Serial.println("IS NULLL !!!!!");
        }

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
