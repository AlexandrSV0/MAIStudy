import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.widgets import Button, TextBox
from matplotlib.colors import LightSource

approximation = 5 # точность отрисовки
alpha = 0.5 # коэфф. отвечающий за затемнение передней поверхности (прозрачность)
appr_max = 11
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

coeffs= (1, 2, 2)  # a, b, c
rx, ry, rz = 1 / np.sqrt(coeffs)

#множества углов
u = np.linspace(-np.pi, np.pi, 40)
v = np.linspace(np.pi/4, np.pi*3/4, 40)
verts =[]

#вычисление точек на каждой из осей с помощью сферических координат
x = rx * np.outer(np.cos(u),np.sin(v))
y = ry * np.outer(np.sin(u), np.sin(v))
z = rz * np.outer(np.ones_like(u), np.cos(v))

#кнопки и поля
def button_callback_remove(event):
    global alpha
    alpha = 1
    ax.plot_surface(x, y, z,  rstride=approximation, cstride=approximation, color='grey', alpha=alpha, edgecolors="black")
    plt.draw()

button_ax_remove = fig.add_axes([0.67, 0.05, 0.31, 0.06])
button_remove = Button(button_ax_remove, "Убрать линии")
button_remove.on_clicked(button_callback_remove)

def button_callback_show(event):
    global alpha
    alpha = 0.5 
    ax.plot_surface(x, y, z,  rstride=approximation, cstride=approximation, color='grey', alpha=alpha, edgecolors="black")
    plt.draw()


button_ax_show = fig.add_axes([0.67, 0.15, 0.31, 0.06])
button_show = Button(button_ax_show, "Показать линии")
button_show.on_clicked(button_callback_show)

#изменение освещения
def change_light(intensitivity):
    light = LightSource() 
    illuminated_surface = light.shade(-z, plt.cm.copper, fraction=float(intensitivity))
    ax.plot_surface(x, y, z,  rstride=approximation, cstride=approximation, color='grey', alpha=alpha, edgecolors="black", antialiased=False, facecolors=illuminated_surface)
    plt.draw()
    
light_box = fig.add_axes([0.200, 0.15, 0.15, 0.06])
text_box_light = TextBox(light_box, "Освещение: ")
text_box_light.on_submit(change_light)


#изменение аппроксимации
def change_approximation(new_approximation):
    global approximation, alpha
    approximation = appr_max - int(new_approximation)
    ax.clear()
    ax.plot_surface(x, y, z, rstride=approximation, cstride=approximation,color='grey', alpha=alpha, edgecolors="black")
    max_radius = max(rx, ry, rz)
    for axis in 'xyz':
        getattr(ax, 'set_{}lim'.format(axis))((-max_radius, max_radius))
    plt.draw()

axbox = fig.add_axes([0.205, 0.05, 0.15, 0.06])
text_box_B = TextBox(axbox, "Аппроксимация: ")
text_box_B.on_submit(change_approximation)
text_box_B.set_val(str(appr_max - approximation))

plt.show()