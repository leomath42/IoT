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

bool push_weather(WeatherStack* pstack, Weather weather)
{
   if(stack_limit_counter < WEATHER_STACK_LIMIT)
   {
        WeatherStack stack;

        stack.weather = weather;
        stack.before = pstack;
        pstack = &stack;    

        stack_limit_counter++;
        return true;
   }
   else {
       return false;
   }
}

Weather* pop_weather(WeatherStack* pstack)
{
    if(pstack != NULL)
    {
        Serial.println(pstack->weather.humidity);
        Weather aux = pstack->weather;
        pstack = pstack->before;
        stack_limit_counter--;

        return &aux;
    };

    return NULL;
}