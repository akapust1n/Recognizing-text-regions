from sys import argv
from pathlib import Path

from keras.preprocessing.image import ImageDataGenerator

if len(argv) != 3:
    print(f'usage: {argv[0]} data_dir epochs')
home_dir = str(Path.home())
data_dir = argv[1]
epochs = int(argv[2])
train_data_dir = home_dir + f'/data/{data_dir}/train'
validation_data_dir = home_dir + f'/data/{data_dir}/validation'
img_size = 224
batch_size = 32
train_samples = 40000
validation_samples = 10000
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
