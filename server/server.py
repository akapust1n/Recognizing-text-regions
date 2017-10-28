import os
from tempfile import NamedTemporaryFile

from cv2 import imwrite
from flask import Flask, request, jsonify, render_template
from werkzeug.contrib.fixers import ProxyFix

from predict import predict

app = Flask(__name__)

TMP_DIR = 'static'


@app.route('/images', methods=['POST'])
def receive_images():
    return jsonify([predict(file) for file in request.files.values()])


@app.route('/')
def demo():
    return render_template('demo.html')


@app.route('/', methods=['POST'])
def demo_post():
    results = []
    for result in (predict(file, True) for file in request.files.getlist("files")):
        temp = NamedTemporaryFile(suffix='.jpg', delete=False, dir=TMP_DIR)
        imwrite(temp.name, result[1])
        name = '/'.join(temp.name.split('/')[-2:])
        results.append((result[0], name))
    return render_template('demo.html', results=results)


@app.route('/clear')
def demo_clear():
    for f in os.listdir(TMP_DIR):
        if f.endswith('.jpg'):
            os.remove(os.path.join(TMP_DIR, f))
    return 'OK'


app.wsgi_app = ProxyFix(app.wsgi_app)
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
