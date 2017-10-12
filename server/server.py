import os
from tempfile import NamedTemporaryFile

from cv2 import imwrite
from flask import Flask, request, jsonify, render_template

from predict import predict

app = Flask(__name__)

TMP_DIR = 'static'


@app.route('/', methods=['POST'])
def receive_images():
    return jsonify([predict(file) for file in request.files.values()])


@app.route('/demo')
def demo():
    return render_template('demo.html')


@app.route('/demo', methods=['POST'])
def demo_post():
    temp = NamedTemporaryFile(suffix='.jpg', delete=False, dir=TMP_DIR)
    result, img = predict(request.files.get('img'), True)
    imwrite(temp.name, img)
    name = '/'.join(temp.name.split('/')[-2:])
    return render_template('demo.html', coordinates=result, image=name)


@app.route('/clear')
def demo_clear():
    for f in os.listdir(TMP_DIR):
        if f.endswith('.jpg'):
            os.remove(os.path.join(TMP_DIR, f))
    return 'OK'


if __name__ == '__main__':
    app.run()
