import numpy
from keras.utils import np_utils
from scipy.misc import imread
from os import path

img_size = 224
test_count = 500


def load_data():
    dataset_path = path.expanduser('~') + '/dataset'
    dataset_info_path = path.expanduser('~') + '/dataset_info'
    dataset_info = []
    with open(dataset_info_path) as f:
        for line in f:
            filename, has_text = line.split()
            dataset_info.append((filename, int(has_text)))
    numpy.random.shuffle(dataset_info)
    image_count = len(dataset_info)
    x = numpy.empty((image_count, img_size, img_size))
    y = numpy.empty((image_count, 1))
    for i in range(image_count):
        filename = dataset_info[i][0]
        has_text = dataset_info[i][1]
        image = imread(dataset_path + '/' + filename, flatten=True, mode='L')
        x[i] = image
        y[i] = has_text
    y = np_utils.to_categorical(y, 2)
    x = x.astype('float32')
    x /= 255
    x = x.reshape(x.shape[0], img_size, img_size, 1)
    x_train = x[:-test_count]
    y_train = y[:-test_count]
    x_test = x[-test_count:]
    y_test = y[-test_count:]
    return x_train, y_train, x_test, y_test
