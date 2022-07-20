from flask import Flask
from flask import request
from flask import jsonify

app = Flask(__name__)


@app.route("/weather", methods=["POST"])
def post():
    print(request.data)
    json = request.get_json()

    print(json)
    return jsonify(json)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
