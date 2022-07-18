#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"


#define TIME_TO_WAIT 10000

long last_read = 0;
long last_time = 0;

char buffer[32];

WeatherStack* stack = newStack();

void setup()
{
    // buffer[30] = '\n';
    // buffer[31] = 0;

    Serial.begin(115200);
    
    setup_weather();
    
    display_setup();
    
    setup_ota();
}

void loop()
{
    handle_ota();

    last_time = millis();

    if(TIME_TO_WAIT < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        bool success = push_weather(stack, weather);

        Weather weather2 = stack->elements[stack->index - 1];
        Weather* pweather = pop_weather(stack);

        if(pweather == NULL)
        {
        }
        else{
            int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f  Temp: %.1f\0", pweather->humidity, pweather->temperature);
            logger_print(buffer);
        }

    }

// OTA, MQTT
// 
// WiFiManager

// Interrupção hardware -> push button 
// time 
// watchdog

}
