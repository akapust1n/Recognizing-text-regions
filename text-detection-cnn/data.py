from keras.preprocessing.image import ImageDataGenerator
from pathlib import Path

home_dir = str(Path.home())
train_data_dir = home_dir + '/data/train'
validation_data_dir = home_dir + '/data/validation'
img_size = 224
batch_size = 32
train_samples = 40000
validation_samples = 10000
epochs = 100
steps_per_epoch = train_samples // batch_size
validation_steps = validation_samples // batch_size
datagen = ImageDataGenerator(rescale=1. / 255)
train_generator = datagen.flow_from_directory(train_data_dir,
                                              target_size=(img_size, img_size),
                                              color_mode='grayscale',
                                              class_mode='binary',
                                              batch_size=batch_size)
validation_generator = datagen.flow_from_directory(validation_data_dir,
                                                   target_size=(img_size, img_size),
                                                   color_mode='grayscale',
                                                   class_mode='binary',
                                                   batch_size=batch_size)
