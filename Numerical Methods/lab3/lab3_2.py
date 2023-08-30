import numpy as np
import matplotlib.pyplot as plt

from imported.lab1_2 import tridiagonal_solve

# вычисляет значение сплайна в точке
def calc_spline_value(a, b, c, d, x):
    return a + b * x + c * x**2 + d * x**3

# вычисляет кубический сплайн - значения всех коэффцициентов и значение проверяемой величины
# s(x) = a + b(x - x_{i-1}) + c(x - x_{i-1})^2 + d(x - x_{i-1})^3
def spline_interpolation(x, y, x_checking):
    assert len(x) == len(y)
    n = len(x)

    # с - коэф.
    h = [x[i] - x[i - 1] for i in range(1, len(x))] # h_i = x_i - x_i-1
    # трехдиагональная матрица для вычисления коэф. c
    A = [[0 for _ in range(len(h)-1)] for _ in range(len(h)-1)]
    A[0][0] = 2 * (h[0] + h[1])
    A[0][1] = h[1]
    A[-1][-2] = h[-2]
    A[-1][-1] = 2 * (h[-2] + h[-1])

    for i in range(1, len(A) - 1):
        A[i][i-1] = h[i-1]
        A[i][i] = 2 * (h[i-1] + h[i])
        A[i][i+1] = h[i]        

    m = [3.0 * ((y[i+1] - y[i]) / h[i] - (y[i] - y[i-1]) / h[i-1]) for i in range(1, len(h))]

    c = [0] + tridiagonal_solve(A, m)

    # а - коэф.
    a = [y[i-1] for i in range(1, n)]

    # b - коэф.
    b = [(y[i] - y[i-1]) / h[i-1] - (h[i-1] / 3.0) * (2.0 * c[i-1] + c[i]) for i in range(1, len(h))]
    b.append((y[-1] - y[-2]) / h[-1] - (2.0 * h[-1] * c[-1]) / 3.0)

    # d - коэф.
    d = [(c[i] - c[i-1]) / (3.0 * h[i-1]) for i in range(1, len(h))]
    d.append(-c[-1] / (3.0 * h[-1]))

    # s(x*)
    spline_res_value = -1
    for interval in range(len(x) - 1):
        if x[interval] <= x_checking < x[interval+1]:
            i = interval
            spline_res_value = calc_spline_value(a[i + 1], b[i + 1], c[i + 1], d[i + 1], x_checking - x[i])
            break
    return a, b, c, d, spline_res_value

# создание графика сплайна
def draw_plot(x_original, y_original, a, b, c, d):
    x, y = [], []

    for i in range(len(x_original) - 1):
        x1 = np.linspace(x_original[i], x_original[i + 1], 10)
        y1 = [calc_spline_value(a[i], b[i], c[i], d[i], j - x_original[i]) for j in x1]
        x.append(x1)
        y.append(y1)

    plt.grid()
    plt.scatter(x_original, y_original, color='r')
    for i in range(len(x_original) - 1):
        plt.plot(x[i], y[i], color='g')
    plt.show()


if __name__ == '__main__':
    x = [-0.4, -0.1, 0.2, 0.5, 0.8]
    y = [1.5823, 1.5710, 1.5694, 1.5472, 1.4435]
    x_checking = 0.1
    a, b, c, d, spline_res_value = spline_interpolation(x, y, x_checking)

    for i in range(len(x) - 1):
        print(f'[{x[i]}; {x[i+1]})')
        print(f's(x) = {a[i]} + {b[i]}(x - {x[i]}) + {c[i]}(x - {x[i]})^2 + {d[i]}(x - {x[i]})^3')
    print(f's(x*) = s({x_checking}) = {spline_res_value}')
    draw_plot(x, y, a, b, c, d)
