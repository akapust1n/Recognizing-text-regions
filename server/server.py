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
    tempFiles = []
    names = []
    print(request.files.getlist("file[]"))
    result, images = predict(request.files.getlist("file[]"), True)
    for index in range(len(images)):
        tempFiles.append(NamedTemporaryFile(
            suffix='.jpg', delete=False, dir=TMP_DIR))

    print("name__", tempFiles[0].name)

    for index in range(len(images)):
        imwrite(tempFiles[index].name, images[index])
        names.append('/'.join(tempFiles[index].name.split('/')[-2:]))
    print("result__", result)

    return render_template('demo.html', coordinates=result, images=names)


@app.route('/clear')
def demo_clear():
    for f in os.listdir(TMP_DIR):
        if f.endswith('.jpg'):
            os.remove(os.path.join(TMP_DIR, f))
    return 'OK'


app.run()
