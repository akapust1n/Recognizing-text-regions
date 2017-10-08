from flask import Flask, request, jsonify
from predict import predict

app = Flask(__name__)


@app.route('/', methods=['POST'])
def receive_images():
    return jsonify([predict(file) for file in request.files.values()])
