#include "Weather.h"

DHT dht(DHTPIN, DHTTYPE);

void setup_weather()
{
    dht.begin();
}

Weather read_weather()
{
    Weather weather;
    weather.humidity = dht.readHumidity();
    weather.temperature = dht.readTemperature(false);
    // weather.humidity = 50.0;
    // weather.temperature = 30.0;

    return weather;
}