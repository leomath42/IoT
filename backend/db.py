from email.policy import default
import uuid
from pymongo import MongoClient
from bson.binary import UuidRepresentation
from uuid import UUID, uuid4
import mongoengine as me
from marshmallow_mongoengine import ModelSchema
import datetime

def connect_database(uri : str):
    """Connect to the mongo database

    Args:
        uri (str): database uri connection 
    """

    try:
        me.connect(host=uri, UuidRepresentation='standard') 
    except Exception as ex:
        ...

class Weather(me.Document):
    _id = me.UUIDField(primary_key=True, default=uuid4 )
    time = me.DateTimeField(default=datetime.datetime.utcnow)
    temperature = me.FloatField()
    humidity = me.FloatField()

class WeatherSchema(ModelSchema):
    class Meta:
        model = Weather


def save_weather_action(weather: Weather):
    try:
        weather.save()
    except Exception as ex:
        ...
    
def get_all_weathers_in_last_24h(limit=1440):
    d = datetime.datetime.utcnow() - datetime.timedelta(1)
    return Weather.objects(time__gte=d).limit(limit)