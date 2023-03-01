from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np
from matplotlib.widgets import Button


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# вершины пирамиды основания
v = np.array([[0, 0, 1.5]])
for i in range(1, 9):
    angle = (2 * np.pi * i) / 8
    x = 2*np.cos(angle)
    y = 2*np.sin(angle)
    v = np.vstack([v, [x, y, 0]])



k = 1 - float(abs((10 - 0) / (20- 0))) #коэфф подобия
#вершины в усеченной плоскости
v_sec = np.array([[0,0,1.5]])
for i in range(1, 9):
    angle = (2 * np.pi * i) / 8
    x = 2* k*np.cos(angle)
    y = 2*k* np.sin(angle)
    v_sec= np.vstack([v_sec, [x, y, 1.5]])

ax.scatter3D(v[:, 0], v[:, 1], v[:, 2]) 
   
# стороны пирамиды
verts = [[v[1],v[2],v[3],v[4],v[5],v[6],v[7], v[8]], [v_sec[1], v_sec[2],v_sec[3],v_sec[4],v_sec[5],v_sec[6],v_sec[7],v_sec[8]], 
        [v[2],v[1], v_sec[1], v_sec[2]], [v[3], v[2], v_sec[2], v_sec[3]], [v[4], v[3], v_sec[3], v_sec[4]], [v[5], v[4],v_sec[4],v_sec[5]], [v[6], v[5], v_sec[5],v_sec[6]], 
        [v[7],v[6], v_sec[6], v_sec[7]], [v[8], v[7], v_sec[7],v_sec[8]], [v[8], v[1], v_sec[1], v_sec[8]]]


# отрисовка
ax.add_collection3d(Poly3DCollection(verts, edgecolors='black', alpha=0.5))


def iButton(event):
    ax.view_init(28, -136)
    plt.draw()

axes_ibutton_add = plt.axes([0.7, 0.05, 0.25, 0.05])
ibutton_add = Button(axes_ibutton_add, 'Изометрическая')
ibutton_add.on_clicked(iButton)


def oButton(event):
    ax.view_init(-2, -36)
    plt.draw()


axes_obutton_add = plt.axes([0.05, 0.05, 0.25, 0.05])
obutton_add = Button(axes_obutton_add, 'Ортографическая')
obutton_add.on_clicked(oButton)

def button_callback_remove(event):
    ax.add_collection3d(Poly3DCollection(verts, edgecolors='black', alpha=1))
    plt.draw()

axes_obutton_remove = plt.axes([0.05, 0.14, 0.2, 0.05])
obutton_remove = Button(axes_obutton_remove, 'Убрать линии')
obutton_remove.on_clicked(button_callback_remove)

def button_callback_show(event):
    ax.add_collection3d(Poly3DCollection(verts, edgecolors='black', alpha=0.5))
    plt.draw()

axes_obutton_show = plt.axes([0.75, 0.14, 0.2, 0.05])
obutton_show = Button(axes_obutton_show, 'Показать линии')
obutton_show.on_clicked(button_callback_show)


plt.show()