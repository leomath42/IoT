#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"


#define TIME_TO_WAIT 60000

long last_read = 0;
long last_time = 0;

char buffer[32];


// void logger_weather(const char* humidity, const char* temperature);

void setup()
{
    buffer[30] = '\n';
    buffer[31] = 0;

    Serial.begin(115200);
    
    setup_weather();
    
    display_setup();
    
    setup_wifi();
}


// char buffer_1[16];
// char buffer_2[16];

// void logger_weather(const char* humidity, const char* temperature)
// {
//     logger_print(buffer_1);
//         logger_print(buffer_2);

// }

void loop()
{
    last_time = millis();

    if(1000 < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f\nTemp: %.1f", weather.humidity, weather.temperature);
        Serial.println("DDDDDDDDDDDDDDDD");
        logger_print(buffer);
        // int ret_2 = snprintf(buffer_2, sizeof buffer_2, "Temperature: %.2f", weather.temperature);
    }

// OTA, MQTT
// 
// WiFiManager

// Interrupção hardware -> push button 
// time 
// watchdog

}
