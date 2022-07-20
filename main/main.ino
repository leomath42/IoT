#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"


#define TIME_TO_WAIT 10000

long last_read = 0;
long last_time = 0;

long pop_last_read = 0;
long pop_last_time = 0;

char buffer[32];

IPAddress server(192, 168, 0, 10);

WeatherStack* stack = newStack();

char* serializeWeather(Weather weather)
{
    char* temp = (char *) malloc(sizeof(char) *256);
    int ret = snprintf(temp, sizeof(char) * 256, "{\"temperature\":\"%.2f\"}", weather.temperature);
    return temp;
}

WiFiClient client;

void setup()
{
    // buffer[30] = '\n';
    // buffer[31] = 0;

    //WiFi.mode(WIFI_AP_STA);

    Serial.begin(115200);
    
    setup_wifi();

    setup_weather();
    
    display_setup();
    
    //setup_ota();
}

void loop()
{
    handle_ota();

    last_time = millis();

    if(5000 < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        bool success = push_weather(stack, weather);

        if(stack != NULL)
        {
            //Serial.printf("Number in stack %i\n", stack->index);
        }
    }

    pop_last_time = millis();

    if(2000 < (pop_last_time - pop_last_read))
    {
        pop_last_read = pop_last_time;

        Weather* pweather = pop_weather(stack);

        if(pweather != NULL)
        {
            //int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f  Temp: %.1f\0", pweather->humidity, pweather->temperature);
            //logger_print(buffer);
            char* aux = serializeWeather(*pweather);
            Serial.printf(aux);
        }

        Serial.println("=======DEBUG============");
        if(client.connect(server, 5000))
        {
            Serial.println("CONNECTED");
            char* post = "POST /weather HTTP/1.1\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 20\r\n"
            "Connection: close\r\n"
            "\r\n"
            "{\"temperature\":30.0}";

            client.write(post);
        }
    }

// OTA, MQTT
// 
// WiFiManager

// Interrupção hardware -> push button 
// time 
// watchdog

}
