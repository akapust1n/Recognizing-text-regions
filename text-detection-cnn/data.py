import numpy
from keras.utils import np_utils
from scipy.misc import imread
from os import path

img_size = 224


def load_data():
    dataset_path = path.expanduser('~') + '/dataset'
    dataset_info_path = path.expanduser('~') + '/dataset_info'
    dataset = []
    with open(dataset_info_path) as f:
        for line in f:
            filename, has_text = line.split()
            image = imread(dataset_path + '/' + filename, flatten=True, mode='L')
            dataset.append((image, int(has_text)))
    numpy.random.shuffle(dataset)
    x = []
    y = []
    for data in dataset:
        x.append(data[0])
        y.append(data[1])
    x = numpy.array(x)
    y = numpy.array(y)
    y = np_utils.to_categorical(y, 2)
    x = x.astype('float32')
    x /= 255
    x = x.reshape(x.shape[0], img_size, img_size, 1)
    test_count = 500
    x_train = x[:-test_count]
    y_train = y[:-test_count]
    x_test = x[-test_count:]
    y_test = y[-test_count:]
    return x_train, y_train, x_test, y_test
