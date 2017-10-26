from pathlib import Path
from sys import argv
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from keras.models import load_model
from keras.preprocessing.image import ImageDataGenerator

home_dir = str(Path.home())
model_path = argv[1]
my_model = load_model(home_dir + '/' + model_path)
notext_data_dir = home_dir + '/notext'
text_data_dir = home_dir + '/text'
img_size = 224
batch_size = 10
test_samples = int(argv[2])
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
values = [(i[0], 1) for i in predict_text] + [(i[0], 0) for i in predict_notext]
values.sort(key=lambda v: v[0], reverse=True)
n = len(values)
current_x = 0
current_y = 0
x = [0]
y = [0]
for value in values:
    if value[1] == 1:
        current_x += 0
        current_y += 1
    else:
        current_x += 1
        current_y += 0
    x.append(current_x)
    y.append(current_y)
max_x = max(x)
max_y = max(y)
x = [i / max_x for i in x]
y = [i / max_y for i in y]
i = 0
aoc = 0
h = 0
while i < n:
    while i < n - 1 and x[i] == x[i + 1]:
        h = y[i + 1]
        i += 1
    aoc += h * (x[i + 1] - x[i])
    i += 1
print(aoc)
plt.plot(x, y)
plt.grid(True)
plt.savefig('roc.png')
