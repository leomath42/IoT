#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"

#define TIME_TO_WAIT 10000
#define POST_HEADER_SIZE 100

long last_read = 0;
long last_time = 0;

long pop_last_read = 0;
long pop_last_time = 0;

char buffer[32];

IPAddress server(192, 168, 0, 10);

WeatherStack *stack = newStack();

// serializeWeather to buffer defining a size to it.
// returns the serialization size.
char *serializeWeather(Weather weather, size_t *length, size_t size)
{
    char *aux = (char *)malloc(sizeof(char) * size);
    *length = snprintf(aux, size, "{\"temperature\":\"%.2f\", \"humidity\":\"%.2f\"}", weather.temperature, weather.humidity);
    return aux;
}

// Do a http post of a weather object.
char *HttpPost(Weather weather)
{
    const char *post_header = "POST /weather HTTP/1.1\r\n"
                              "Content-Type: application/json\r\n"
                              "Connection: close\r\n"
                              "Content-Length: %i\r\n"
                              "\r\n"
                              "%s";

    // serialize weather and get the length of the serialization.
    size_t length = 0;
    char *weather_buffer = serializeWeather(weather, &length, 48);

    int buffer_size = sizeof(char) * (POST_HEADER_SIZE + length);
    char *http_post_buffer = (char *)malloc(sizeof(char) * buffer_size);

    int ret = snprintf(http_post_buffer, buffer_size, post_header, length, weather_buffer);
    free(weather_buffer);

    return http_post_buffer;
}

WiFiClient client;

void setup()
{
    Serial.begin(115200);
    setup_wifi();
    setup_weather();
    display_setup();
    // setup_ota();
}

void loop()
{
    handle_ota();
    last_time = millis();
    if (100 < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        bool success = push_weather(stack, weather);
    }

    pop_last_time = millis();

    if (100 < (pop_last_time - pop_last_read))
    {
        pop_last_read = pop_last_time;

        if (stack->index == WEATHER_STACK_LIMIT)
        {
            // int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f  Temp: %.1f\0", pweather->humidity, pweather->temperature);
            // logger_print(buffer);
            if (client.connect(server, 5000))
            {
                // compute the mean of all weather mensuration::
                Weather aux;
                for (int i = stack->index; i> 0; i--)
                {
                    Weather *pweather = pop_weather(stack);

                    aux.humidity+= pweather->humidity;
                    aux.temperature+= pweather->temperature;
                }
                aux.humidity/= stack->maxSize;
                aux.temperature/= stack->maxSize;
                //uint32_t usage = system_get_free_heap_size();

                // Post to server
                char *post = HttpPost(aux);
                client.write(post);
                free(post);
                client.flush();
                client.stop();
            }
        }
    }

    // OTA, MQTT
    //
    // WiFiManager

    // Interrupção hardware -> push button
    // time
    // watchdog
}
