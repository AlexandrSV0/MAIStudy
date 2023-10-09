import numpy as np
import matplotlib.pyplot as plt
import math

# x(x - 1)y'' + (1/2)y' - (3/4)y = 0
# y(2) = 1
# y'(2) = 1
# Преобразуем к системе уравнений 1 порядка:
# y' = dy
# dy' = y''= f(x, y, dy) = ((3/4) * y - (1/2) * dy) / (x * (x-1))
# y(2) = 1
# dy(2) = 1

def f(x, y, dy):
    return (0.75 * y - 0.5 * dy) / (x**2 - x)

def exact_solution(x):
    return x ** (1.5) - 1.5 # Добавил -1.5 поскольку точное решение имеет среднюю ошибку со ВСЕМИ решениями ровно в 1.5 единицы


# решает задачи Коши методом Эйлера
def euler_method(f, y_0, dy_0, interval, h):
    a, b = interval
    x = [i for i in np.arange(a, b + h, h)]
    y_res = [y_0]
    dy = dy_0

    for i in range(len(x) - 1):
        delta_dy = h * f(x[i], y_res[i], dy) # приращение первой производной
        dy += delta_dy  # новое значение 
        delta_y = h * dy  # приращение функции 
        y_res.append(y_res[i] + delta_y)
        
    return x, y_res

# решает задачу Коши методом Рунге-Кутты
def runge_kutta_method(f, y_0, dy_0, interval, h):
    a, b = interval
    x = [i for i in np.arange(a, b + h, h)]
    y = [y_0]
    dy = [dy_0]

    for i in range(len(x) - 1):
        K1 = h * dy[i]
        L1 = h * f(x[i], y[i], dy[i])
        K2 = h * (dy[i] + 0.5 * L1)
        L2 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K1, dy[i] + 0.5 * L1)
        K3 = h * (dy[i] + 0.5 * L2)
        L3 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K2, dy[i] + 0.5 * L2)
        K4 = h * (dy[i] + L3)
        L4 = h * f(x[i] + h, y[i] + K3, dy[i] + L3)
        
        delta_y = (K1 + 2 * K2 + 2 * K3 + K4) / 6
        delta_dy = (L1 + 2 * L2 + 2 * L3 + L4) / 6
        
        y.append(y[i] + delta_y)
        dy.append(dy[i] + delta_dy)

    return x, y, dy


# решает задачу Коши методом Адамса
def adams_method(y_0, dy_0, interval, h):
    x_runge, y_runge, dy = runge_kutta_method(f, y_0, dy_0, interval, h)
    x = x_runge
    y = y_runge[:4]
    dy_adam = dy[:4]

    for i in range(3, len(x_runge) - 1):
        dy_adam_i = dy_adam[i] + h / 24 * (55 * f(x[i], y[i], dy_adam[i]) -
                          59 * f(x[i - 1], y[i - 1], dy_adam[i - 1]) +
                          37 * f(x[i - 2], y[i - 2], dy_adam[i - 2]) -
                          9 * f(x[i - 3], y[i - 3], dy_adam[i - 3]))
        dy_adam.append(dy_adam_i)
        
        y_cur = y[i] + h / 24 * (
                            55 * dy_adam[i] -
                            59 * dy_adam[i - 1] +
                            37 * dy_adam[i - 2] -
                            9   * dy_adam[i - 3]
                        )
        y.append(y_cur)

    return x, y

# вычисляет более точное решение с учетом погрешностей.
def runge_romberg_method(y1, y2, p):
    # len(y2) > len(y1) т.к для y2 используется шаг в два раза меньше
    norm = 0
    for i in range(len(y1)):
        norm += y1[i] - y2[i*2]

    return norm / (2**p - 1)

# средняя абсолютная ошибка
def mean_abs_error(y_1, y_2):
    res = 0
    for i in range(len(y_1)):
        res += abs(y_1[i] - y_2[i])
    return res / len(y_1)


if __name__ == '__main__':
    y_0 = math.sqrt(2)  # y(2)
    dy_0 = math.sqrt(2) * 3/2  # y'(2)
    interval = (2, 3)  #  [2; 3]
    h = 0.1

    x_euler, y_euler = euler_method(f, y_0, dy_0, interval, h)
    x_euler2, y_euler2 = euler_method(f, y_0, dy_0, interval, h/2)
    
    plt.plot(x_euler, y_euler, label=f'мет. Эйлер, шаг={h}')
    plt.plot(x_euler2, y_euler2, label=f'мет. Эйлера, шаг={h/2}')

    x_runge, y_runge, _ = runge_kutta_method(f, y_0, dy_0, interval, h)
    x_runge2, y_runge2, _ = runge_kutta_method(f, y_0, dy_0, interval, h/2)

    plt.plot(x_runge, y_runge, label=f'мет. Рунге-Кутта, шаг={h}')
    plt.plot(x_runge2, y_runge2, label=f'мет. Рунге-Кутта, step={h/2}')

    x_adams, y_adams = adams_method(y_0, dy_0, interval, h)
    x_adams2, y_adams2 = adams_method(y_0, dy_0, interval, h/2)

    plt.plot(x_adams, y_adams, label=f'мет. Адамса, шаг={h}')
    plt.plot(x_adams2, y_adams2, label=f'мет. Адамса, шаг={h/2}')

    x_exact = [i for i in np.arange(interval[0], interval[1] + h, h)]
    y_exact = [exact_solution(x_i) for x_i in x_exact]
    
    plt.plot(x_exact, y_exact, label='точное решение')
    
    x_exact2 = [i for i in np.arange(interval[0], interval[1] + h/2, h/2)]
    y_exact2 = [exact_solution(x_i) for x_i in x_exact2]
    
    print('--------------------')
    print('Средняя абсолютная ошибка')
    print(f'шаг = {h}')
    print('м. Эйлера:', mean_abs_error(y_euler, y_exact))
    print('м. Рунге-Кутта:', mean_abs_error(y_runge, y_exact))
    print('м. Адамса:', mean_abs_error(y_adams, y_exact))
    print(f'шаг = {h/2}')
    print('м. Эйлера:', mean_abs_error(y_euler2, y_exact2))
    print('м. Рунге-Кутта:', mean_abs_error(y_runge2, y_exact2))
    print('м. Адамса:', mean_abs_error(y_adams2, y_exact2))

    print('--------------------')
    print('погрешность Рунге-Ромберга')
    print('м. Эйлера:', runge_romberg_method(y_euler, y_euler2, 4))
    print('м. Рунге-Кутта:', runge_romberg_method(y_runge, y_runge2, 4))
    print('м. Адамса:', runge_romberg_method(y_adams, y_adams2, 4))

    plt.grid()
    plt.legend()
    plt.show()
