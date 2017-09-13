from keras.models import load_model
from scipy.misc import imread, imresize
import numpy

model = load_model('my_model.h5')
model.load_weights('my_model_weights.h5')
while True:
    try:
        path = input('img path: ')
        image = imread(path, True, 'L')
    except OSError:
        print('OSError')
        continue
    image = imresize(image, (224, 224))
    image = image.astype('float32')
    image /= 255
    image = image.reshape(1, 224, 224, 1)
    prediction = model.predict(image)
    print(numpy.argmax(prediction))
