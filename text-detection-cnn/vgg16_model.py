from keras.applications.vgg16 import VGG16
from keras.layers import Dense, Flatten, Dropout
from keras.models import Model
from keras import Input
from data import img_size, train_generator, validation_generator, steps_per_epoch, validation_steps, epochs

vgg16_model = VGG16(include_top=False, weights=None, input_shape=(img_size, img_size, 1))
model_in = Input(shape=(img_size, img_size, 1))
model_out = vgg16_model(model_in)
model_out = Flatten()(model_out)
model_out = Dense(64, activation='relu')(model_out)
model_out = Dropout(0.25)(model_out)
model_out = Dense(1, activation='sigmoid')(model_out)
model = Model(input=model_in, output=model_out)
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit_generator(train_generator,
                    steps_per_epoch=steps_per_epoch,
                    epochs=epochs,
                    validation_data=validation_generator,
                    validation_steps=validation_steps)
model.save('vgg16_model.h5')
model.save_weights('vgg16_model_weights.h5')
