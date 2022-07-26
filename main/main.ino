#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "src/WiFiHandler/WiFiHandler.h"
#include "src/Logger/Logger.h"
#include "src/Weather/Weather.h"

//================ preprocessors macros defines ================= 
#define TIME_TO_WAIT_READ 2000
#define TIME_TO_WAIT_POST 2000
#define NUMBER_OF_MEASUREMENTS 15
#define INTERRUPT_PIN D8
#define INTERRUPT_TASK_TIME 10000
#define POST_HEADER_SIZE 155

#define ENV 1  // Actual env
#define PROD 1 // PROD env
#define DEV 0  // DEV env

#if ENV == PROD

#define API_HOST "http://leomath42-weather-iot.herokuapp.com"
#define HOST "leomath42-weather-iot.herokuapp.com"
#define API_PORT 80

#elif ENV == DEV

#define API_HOST "192.168.0.10"
#define API_PORT 5000
#define HOST "192.168.0.10"

#endif

// defines time schedule interrupts to Read Weather Operation.
unsigned long last_read = 0;
unsigned long last_time = 0;

// defines time schedule interrupts to POP/Post Data Operation.
unsigned long pop_last_read = 0;
unsigned long pop_last_time = 0;

// Defines logger buffer size;
char logger_buffer[32];


// Create a weather stack to keep measurements values.
WeatherStack *stack = newStack();

// Create an WiFiClient object from c++
WiFiClient client;


// Interrupt state logic::
bool interruptState = false;
unsigned long interruptTaskTime = 0;
unsigned long interrupt_last_read = 0;
unsigned long interrupt_last_time = 0;

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
    const char *post_header = "POST /api/v1/weather HTTP/1.1\r\n"
                              "Host: leomath42-weather-iot.herokuapp.com\r\n"
                              "Connection: close\r\n"
                              "Content-Type: application/json\r\n"
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

// Execute interruption action on PIN D8:
// Its necessary to used ICACHE_RAM_ATTR, otherwise 
// the esp8266 will crash, the cause can be concurrence, cache error.
// 
// ref.: https://stackoverflow.com/questions/58113937/esp8266-arduino-why-is-it-necessary-to-add-the-icache-ram-attr-macro-to-isrs-an
void ICACHE_RAM_ATTR doActionInInterrupt()
{  if(!interruptState)
    {
        Serial.println("Initing interrupt....");
        Weather *pweather = pop_weather(stack);

        if(pweather == NULL)
        {
            Weather weather = read_weather();
            pweather = &weather;
        }

        int ret = snprintf(logger_buffer, sizeof logger_buffer, "Humidity: %.1f  Temp: %.1f\0", pweather->humidity, pweather->temperature);
        push_weather(stack, *pweather);
        logger_print(logger_buffer);
        interruptState = true;
    }
    interrupt_last_read = millis();
}

// void setup
void setup()
{
    pinMode(INTERRUPT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), doActionInInterrupt, FALLING);
    Serial.begin(115200);
    setup_wifi("ESP", "123456789");
    setup_weather();
    display_setup();
    setup_ota("esp", "ota-teste");
}

// void loop
void loop()
{
    handle_ota();

    if(interruptState)
    {
        interrupt_last_time = millis();
        if(interrupt_last_time >= (INTERRUPT_TASK_TIME + interrupt_last_read) )
        {
            interruptState = false;
            display_clear(); 
        }
    }

    last_time = millis();
    if (TIME_TO_WAIT_READ < (last_time - last_read))
    {
        last_read = last_time;
        Weather weather = read_weather();

        bool success = push_weather(stack, weather);
    }

    pop_last_time = millis();
    if (TIME_TO_WAIT_POST < (pop_last_time - pop_last_read))
    {
        pop_last_read = pop_last_time;

        if (stack->index > NUMBER_OF_MEASUREMENTS)
        {
            // int ret = snprintf(buffer, sizeof buffer, "Humidity: %.1f  Temp: %.1f\0", pweather->humidity, pweather->temperature);
            // logger_print(buffer);
            if (client.connect(API_HOST, API_PORT))
            {
                // compute the mean of all weather mensuration::
                Weather aux;
                int total = stack->index;
                for (int i = stack->index; i > 0; i--)
                {
                    Weather *pweather = pop_weather(stack);

                    aux.humidity += pweather->humidity;
                    aux.temperature += pweather->temperature;
                }
                aux.humidity /= total;
                aux.temperature /= total;
                // uint32_t usage = system_get_free_heap_size();
                //  Post to server
                char *post = HttpPost(aux);
                client.write(post);
                Serial.println(client.readString());
                delay(4000);
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
