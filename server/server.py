from flask import Flask, request

app = Flask(__name__)


@app.route('/image', methods=['POST'])
def receive_images():
    image = request.files.get('')
