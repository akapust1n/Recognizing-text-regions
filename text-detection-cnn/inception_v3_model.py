from keras import Input
from keras.applications.inception_v3 import InceptionV3
from keras.layers import Dense, Flatten, Dropout
from keras.models import Model
from data import img_size, train_generator, validation_generator, steps_per_epoch, validation_steps, epochs

inception_v3_model = InceptionV3(include_top=False, weights=None, input_shape=(data.img_size, data.img_size, 1))
model_in = Input(shape=(img_size, img_size, 1))
model_out = inception_v3_model(model_in)
model_out = Flatten()(model_out)
model_out = Dense(64, activation='relu')(model_out)
model_out = Dropout(0.25)(model_out)
model_out = Dense(1, activation='sigmoid')(model_out)
model = Model(input=model_in, output=model_out)
model.compile(loss='categorical_crossentropy', optimizer='sgd', metrics=['accuracy'])
model.fit_generator(train_generator,
                    steps_per_epoch=steps_per_epoch,
                    epochs=epochs,
                    validation_data=validation_generator,
                    validation_steps=validation_steps)
model.save('inception_v3_model.h5')
