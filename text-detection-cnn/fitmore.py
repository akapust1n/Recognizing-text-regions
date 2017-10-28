from pathlib import Path
from sys import argv

from keras.models import load_model
from keras.preprocessing.image import ImageDataGenerator

if len(argv) != 6:
    print(f'usage: {argv[0]} model_path data_path train_samples validation_samples epochs')
home_dir = str(Path.home())
model_path = str(Path.home()) + '/' + argv[1]
data_path = str(Path.home()) + '/' + argv[2]
train_samples = int(argv[3])
validation_samples = int(argv[4])
epochs = int(argv[5])

train_data_dir = data_path + '/train'
validation_data_dir = data_path + '/validation'
img_size = 224
batch_size = 32
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

my_model = load_model(model_path)
my_model.fit_generator(train_generator,
                       steps_per_epoch=steps_per_epoch,
                       epochs=epochs,
                       validation_data=validation_generator,
                       validation_steps=validation_steps)
my_model.save(model_path.split('/')[-1][:-3] + '_fitmore.h5')
