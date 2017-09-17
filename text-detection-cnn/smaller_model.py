from keras.layers import Dense, Flatten, Dropout
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.models import Sequential
import data


def fit(x_train, y_train, x_test, y_test, batch_size, epochs):
    model = Sequential()
    model.add(Conv2D(32, (3, 3), input_shape=(data.img_size, data.img_size, 1), activation='relu', padding='same'))
    model.add(Conv2D(32, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))
    model.add(Flatten())
    model.add(Dense(512, activation='relu'))
    model.add(Dense(2, activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer='sgd', metrics=['accuracy'])
    model.fit(x_train, y_train, batch_size=batch_size, epochs=epochs, validation_split=0.1, shuffle=True)
    model.save('smaller_model.h5')
    model.save_weights('smaller_model_weights.h5')
    scores = model.evaluate(x_test, y_test, verbose=0)
    print('Test accuracy: ' + scores[1])
    return model
