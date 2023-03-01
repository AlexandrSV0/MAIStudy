import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import TextBox

# num_a = int(input("Введите константу A: "))
num_a = 1
# num_B = int(input("Введите константу B: "))
num_B = 3
# num = int(input("Введите количество точек графика: "))
num = 50
plt.title('$y^2 = x^3 / (x - a)$')
plt.xlabel('x')
plt.ylabel('y')

x = np.linspace(0, num_B, num,True)
y = pow(pow(x,3) / (x - num_a), 0.5)
y1 = -pow(pow(x,3)/ (x -num_a), 0.5)

plt.grid()
plt.plot(x, y, '-b', x, y1, '-b')
plt.show()