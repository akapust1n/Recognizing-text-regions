from keras.models import load_model
from scipy.misc import imread, imresize
import numpy
import time

model = load_model('my_model.h5')
model.load_weights('my_model_weights.h5')
while True:
    try:
        path = input('img path: ')
        start_time = time.time()
        image = imread(path, True, 'L')
    except OSError:
        print('OSError')
        continue
    file_read_time = time.time()
    image = imresize(image, (224, 224))
    image = image.astype('float32')
    image /= 255
    image = image.reshape(1, 224, 224, 1)
    prediction = model.predict(image)
    end_time = time.time()
    print('has text' if numpy.argmax(prediction) == 1 else 'no text')
    print('elapsed time (s): ', end_time - start_time)
    print('calculation time (s): ', end_time - file_read_time)
    print('file read time (s): ', file_read_time - start_time)
    print()
