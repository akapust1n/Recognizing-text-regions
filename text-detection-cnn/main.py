import numpy
from keras.layers import Dense, Flatten, Dropout
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.models import Sequential
from keras.utils import np_utils
from scipy.misc import imread
from os import path

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
img_size = x.shape[1]
x = x.reshape(x.shape[0], img_size, img_size, 1)
test_count = 500
x_train = x[:-test_count]
y_train = y[:-test_count]
x_test = x[-test_count:]
y_test = y[-test_count:]
model = Sequential()
model.add(Conv2D(32, (3, 3), input_shape=(img_size, img_size, 1), activation='relu', padding='same'))
model.add(Conv2D(32, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))
model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(512, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(2, activation='softmax'))

model.compile(loss='categorical_crossentropy', optimizer='sgd', metrics=['accuracy'])

model.fit(x_train, y_train, batch_size=10, epochs=140, validation_split=0.1, shuffle=True)

model.save('my_model.h5')
model.save_weights('my_model_weights.h5')

scores = model.evaluate(x_test, y_test, verbose=0)
print(scores[1])
