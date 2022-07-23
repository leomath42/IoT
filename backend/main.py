from argparse import Namespace
from flask import Flask
from flask import Blueprint
from flask import request
from flask import jsonify
from flask_restx import Api
from flask_restx import Resource, Namespace
from flask_cors import CORS

from db import *
from db import get_all_weathers_in_last_24h 

app = Flask(__name__)
blueprint = Blueprint('api', __name__)
api = Api(blueprint)
app.register_blueprint(blueprint, url_prefix='/api/v1')
CORS(app)

connect_database("mongodb://127.0.0.1:27017/database")

@api.route("/weather")
class Weather(Resource):
    def post(self):
        schema = WeatherSchema()
        body = request.get_json()

        print(body)
        result = schema.load(body).save()
        print(schema.dump(result))
        return schema.dump(result)

    def get(self):
        schema = WeatherSchema()
        result  = get_all_weathers_in_last_24h()
        return schema.dump(result, many=True)

@api.route("/watchdog")
class Watchdog(Resource):
    def post(self):
        ...
    
    def get(self):
        ...


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
