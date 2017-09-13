from keras import Input
from keras.applications.inception_v3 import InceptionV3
from keras.layers import Dense, Flatten, Dropout
from keras.models import Model
import data


def fit(x_train, y_train, x_test, y_test):
    inception_v3_model = InceptionV3(include_top=False, weights=None, input_shape=(data.img_size, data.img_size, 1))
    model_in = Input(shape=(data.img_size, data.img_size, 1))
    model_out = inception_v3_model(model_in)
    model_out = Flatten()(model_out)
    model_out = Dense(4096, activation='relu')(model_out)
    model_out = Dropout(0.1)(model_out)
    model_out = Dense(4096, activation='relu')(model_out)
    model_out = Dropout(0.1)(model_out)
    model_out = Dense(2, activation='softmax')(model_out)
    model = Model(input=model_in, output=model_out)
    model.compile(loss='categorical_crossentropy', optimizer='sgd', metrics=['accuracy'])
    model.fit(x_train, y_train, batch_size=10, epochs=100, validation_split=0.1, shuffle=True)
    model.save('inception_v3_model.h5')
    model.save_weights('inception_v3_model_weights.h5')
    scores = model.evaluate(x_test, y_test, verbose=0)
    print('Test accuracy: ' + scores[1])
    return model
