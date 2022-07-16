#ifndef WEATHER_H
#define WEATHER_H

#include <DHT.h>

#define DHTPIN 9 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
#define WEATHER_STACK_LIMIT 60

typedef struct 
{
    float temperature;
    float humidity;
} Weather;

typedef struct WeatherStack WeatherStack;                                      

struct WeatherStack                                                            
{                                                                              
    /* data */                                                                 
    Weather weather;                                                           
    WeatherStack* before;                                                      
};                                                                             

void setup_weather();

Weather read_weather();

Weather* pop_weather(WeatherStack* pstack);

bool push_weather(WeatherStack* stack, Weather weather);

#endif WEATHER_H