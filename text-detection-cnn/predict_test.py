from pathlib import Path

from keras.models import load_model
from keras.preprocessing.image import ImageDataGenerator

THRESHOLD = 0.9

home_dir = str(Path.home())
my_model = load_model(home_dir + '/my_model/my_model.h5')
test_data_dir = home_dir + '/test_data'
img_size = 224
batch_size = 8
test_samples = 6000
steps = test_samples // batch_size
datagen = ImageDataGenerator(rescale=1. / 255)
test_generator = datagen.flow_from_directory(test_data_dir,
                                             target_size=(img_size, img_size),
                                             color_mode='grayscale',
                                             class_mode='binary',
                                             batch_size=batch_size)
results = my_model.predict_generator(test_generator, steps)
print("Accuracy:", len([i for i in results if i[0] > THRESHOLD]) / len(results))
