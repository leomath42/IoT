#ifndef WEATHER_H
#define WEATHER_H

#include <DHT.h>

#define DHTPIN 9 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

typedef struct 
{
    float temperature;
    float humidity;
} Weather;

void setup_weather();

Weather read_weather();

#endif WEATHER_H