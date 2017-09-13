from keras.utils import np_utils
import sys
from keras.preprocessing import image
from keras.models import load_model
import numpy as np

print("Загружаю сеть из файлов")
# Загружаем данные об архитектуре сети
model = load_model('my_model.h5')

# Создаем модель
# Загружаем сохраненные веса в модель
model.load_weights("my_model_weights.h5")
print("Загрузка сети завершена")

# Компилируем загруженную модель
model.compile(loss="categorical_crossentropy", optimizer="SGD", metrics=["accuracy"])
print(sys.argv[1])
img_path = sys.argv[1]
img = image.load_img(img_path, target_size=(224, 224), grayscale=True)
x = image.img_to_array(img)
x = 255 -x
x /= 255
x = np.expand_dims(x, axis=0)
#x = preprocess_input(x)

prediction = model.predict(x)
prediction = np.argmax(prediction, axis=1)
print(prediction)
