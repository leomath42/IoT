#include "Weather.h"

DHT dht(DHTPIN, DHTTYPE);

uint32 stack_limit_counter = 0;

void setup_weather()
{
    dht.begin();
}

Weather read_weather()
{
    Weather weather;
    weather.humidity = dht.readHumidity();
    weather.temperature = dht.readTemperature(false);
    return weather;
}

bool push_weather(WeatherStack *pstack, struct Weather weather)
{
    if (pstack->index < WEATHER_STACK_LIMIT)
    {
        pstack->elements[pstack->index] = weather;
        pstack->index++;
        return true;
    }
    else
    {
        return false;
    }
}

Weather* pop_weather(WeatherStack *pstack)
{
    if (pstack == NULL || pstack->index == 0) 
    {
        return NULL;
    };

    Weather* pweather = (pstack->elements + pstack->index - 1);

    pstack->index--;

    return pweather;
}

WeatherStack* newStack()
{
    WeatherStack* stack = (WeatherStack *) malloc(sizeof(WeatherStack));
    stack->index = 0;
    stack->maxSize = WEATHER_STACK_LIMIT;
    stack->elements = (Weather *) malloc(sizeof(Weather) * WEATHER_STACK_LIMIT);

    return stack;
}