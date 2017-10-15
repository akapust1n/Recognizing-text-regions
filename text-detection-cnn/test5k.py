from pathlib import Path

from keras.models import load_model
from keras.preprocessing.image import ImageDataGenerator

THRESHOLD = 0.5

home_dir = str(Path.home())
my_model = load_model(home_dir + '/resnet18/resnet18.h5')
notext_data_dir = home_dir + '/notext'
text_data_dir = home_dir + '/text'
img_size = 224
batch_size = 10
test_samples = 5000
steps = test_samples // batch_size
datagen = ImageDataGenerator(rescale=1. / 255)
notext_generator = datagen.flow_from_directory(notext_data_dir,
                                               target_size=(img_size, img_size),
                                               color_mode='grayscale',
                                               class_mode='binary',
                                               batch_size=batch_size)
text_generator = datagen.flow_from_directory(text_data_dir,
                                             target_size=(img_size, img_size),
                                             color_mode='grayscale',
                                             class_mode='binary',
                                             batch_size=batch_size)
predict_notext = my_model.predict_generator(notext_generator, steps)
predict_text = my_model.predict_generator(text_generator, steps)
print("fp:", len([i for i in predict_notext if i[0] > THRESHOLD]) / len(predict_notext))
print("fn:", len([i for i in predict_text if i[0] < THRESHOLD]) / len(predict_text))
