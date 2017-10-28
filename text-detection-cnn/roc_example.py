# classes -1,1 binary
import numpy as np
import matplotlib.pyplot as plt

# Вывод на экран текущей версии библиотеки matplotlib
values = [[-1, 0.5], [-1, 0.1], [-1, 0.2],
          [1, 0.6], [1, 0.2], [1, 0.3], [-1, 0]]
values = sorted(values, key=lambda x: x[1], reverse=True)
threshold = 0.25
size = len(values)
print(size)

plus_counter = 0  # редполагаем, что выборка взвешенная
minus_counter = 0
for x in values:
    if(x[0] == -1):
        minus_counter += 1
    else:
        plus_counter += 1

fpr = []
tpr = []
auc = 0

for i in range(0, size):
    fpr.append(0)
    tpr.append(0)

for i in range(0, size - 1):
    x = values[i][1]
   # print(((x >= threshold) and (values[i][0] == 1)))
    if((values[i][0] == -1)):
        fpr[i + 1] = fpr[i] + 1.0 / minus_counter
        tpr[i + 1] = tpr[i]
        auc = auc + 1 / minus_counter * tpr[i + 1]
    else:
        fpr[i + 1] = fpr[i]
        tpr[i + 1] = tpr[i] + 1.0 / plus_counter

print(auc)
print(values)
print(fpr, tpr)

plt.plot(fpr, tpr)
plt.grid(True)  # Сетка
plt.show()  # Показать график
