from my_model import fit
from data import load_data

x_train, y_train, x_test, y_test = load_data()
fit(x_train, y_train, x_test, y_test, 16, 200)
