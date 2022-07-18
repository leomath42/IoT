#ifndef WEATHER_H
#define WEATHER_H

#include <DHT.h>

#define DHTPIN 9 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
#define WEATHER_STACK_LIMIT 60

typedef struct Weather Weather;
struct Weather 
{
    float temperature;
    float humidity;
};

typedef struct WeatherStack WeatherStack;                                      

struct WeatherStack                                                            
{                                                                              
    int index;
    int maxSize;
    Weather* elements;                                                           
};                                                                             

void setup_weather();

Weather read_weather();

WeatherStack* newStack();

Weather* pop_weather(WeatherStack* pstack);

bool push_weather(WeatherStack* stack, Weather weather);

#endif WEATHER_H