import numpy as np
import matplotlib.pyplot as plt

from imported.lab1_2 import tridiagonal_solve
from imported.lab3_4 import df
from imported.lab4_1 import euler_method, runge_romberg_method, mean_abs_error

# x(2x+1)y''+2(x+1)y'-2y=0,
# y'(1) = 0
# y(3) - y'(3) = 31/9

# Преобразуем к системе уравнений
# y' = dy
# dy' = f(x, y, z) = (-2*(x + 1)*z + 2 * y) / (x * (2*x  + 1))
# y'(1) = 0
# y(3) - y'(3) = 31/9


EPS = 0.00001

def f(x, y, z):
    return (-2*(x + 1)*z + 2 * y) / (x * (2*x  + 1))


# функции для конечно-разностного метода
# y'' + p_fd(x)y' + q_fd(x)y = f_fd(x)
def p(x):
    return (2*(x+1)) / (x*(2*x + 1))


def q(x):
    return -2 / (x*(2*x + 1))

def f_fd(x):
    return 0


def exact_solution(x):
    return x + 1 + 1/x

def get_n(n_prev, n, ans_prev, ans, b, delta, gamma, y1):
    x, y = ans_prev[0], ans_prev[1]
    dy = df(x, y, b)
    phi_n_prev = delta * y[-1] + gamma * dy - y1
    x, y = ans[0], ans[1]
    dy = df( x, y, b)
    phi_n = delta * y[-1] + gamma * dy - y1
    return n - (n - n_prev) / (phi_n - phi_n_prev) * phi_n


def not_converge(x, y, b, delta, gamma, y1):
    dy = df(x, y, b)
    return abs(delta * y[-1] + gamma * dy - y1) > EPS


def shooting_method(f, coefs, y0, y1, interval, h):
    alpha, beta, delta, gamma = coefs
    _, b = interval
    n_prev, n = 1.2, 1.0
    dy = (y0 - alpha * n_prev) / beta
    x_prev, y_prev = euler_method(f, n_prev, dy, interval, h)[:2]
    dy = (y0 - alpha * n) / beta
    x,y = euler_method(f, n, dy, interval, h)[:2]

    while not_converge(x, y, b, delta, gamma, y1):
        n, n_prev = get_n(n_prev, n, (x_prev, y_prev), (x,y), b, delta, gamma, y1), n
        (x_prev, y_prev) = (x, y)
        dy = (y0 - alpha * n) / beta
        x,y = euler_method(f, n, dy, interval, h)[:2]

    return x,y

# конечно-разностный метод для решения краевой задачи для ОДУ 2 порядка
def finite_difference_method(f_fd, y0, yn, coefs, interval, h):
    alpha, beta, delta, gamma = coefs
    A = []
    B = []
    B.append(y0*h)
    a, b = interval
    x = np.arange(a, b + h, h)
    n = len(x)

    # создание трехдиаг. матрицы
    # 0-ая строка
    row = [0 for _ in range(n)]
    row[0] = alpha*h - beta
    row[1] = beta
    A.append(row)

    for i in range(1, n - 1):
        B.append(h**2 * f_fd(x[i]))
        
        row = [0 for _ in range(n)]
        row[i-1] = 1 - p(x[i]) * h/2
        row[i] = q(x[i]) * h**2 - 2
        row[i+1] = 1 + p(x[i]) * h/2
        A.append(row)
    B.append(yn * h)

    # крайняя строка
    row = [0 for _ in range(n)]
    row[-1] = delta*h + gamma
    row[-2] = -gamma
    A.append(row)
    
    y = tridiagonal_solve(A, B) # решение СЛАУ с трехд. матрицей
    return x, y


if __name__ == '__main__':
    interval = (1.0001, 3)  
    y0 = 0 # y'(1) = 0
    y1 = 31/9 # y(3) - y'(3) = 31/9
    h = 0.1
    alpha, beta, delta, gamma = 0, 1,1,-1
    
    x_shooting, y_shooting = shooting_method(f, (alpha, beta, delta, gamma), y0, y1, interval, h)
    x_shooting2, y_shooting2 = shooting_method(f, (alpha, beta, delta, gamma), y0, y1, interval, h / 2)

    plt.plot(x_shooting, y_shooting, label=f'метод стрельбы, шаг={h}')
    plt.plot(x_shooting2, y_shooting2, label=f'метод стрельбы, шаг={h / 2}')

    x_fd, y_fd = finite_difference_method(f_fd, y0, y1, (alpha, beta, delta, gamma), interval, h)
    x_fd2, y_fd2 = finite_difference_method(f_fd, y0, y1, (alpha, beta, delta, gamma), interval, h / 2)

    plt.plot(x_fd, y_fd, label=f'конечно-разностный метод, шаг={h}')
    plt.plot(x_fd2, y_fd2, label=f'конечно-разностный метод, шаг={h / 2}')

    x_exact = [i for i in np.arange(interval[0], interval[1] + h, h)]
    x_exact2 = [i for i in np.arange(interval[0], interval[1] + h / 2, h / 2)]
    y_exact = [exact_solution(x_i) for x_i in x_exact]
    y_exact2 = [exact_solution(x_i) for x_i in x_exact2]

    plt.plot(x_exact, y_exact, label='точное решение')

    print('--------------')
    print('Средняя абсолютная ошибка')
    print(f'* шаг = {h}')
    print('конечно-разностный м.:', mean_abs_error(y_fd, y_exact))
    print(f'* шаг = {h / 2}')
    print('конечно-разностный м:', mean_abs_error(y_fd2, y_exact2))
    print(f'* шаг = {h}')
    print('м. Стрельбы:', mean_abs_error(y_shooting, y_exact))
    print(f'* шаг = {h / 2}')
    print('м. Стрельбы:', mean_abs_error(y_shooting2, y_exact2))
    
    print('--------------')
    print('точность Рунге-Ромберга')
    print('конечно-разностный м:', runge_romberg_method(y_fd, y_fd2, 4))
    print('м. Стрельбы:', runge_romberg_method(y_shooting, y_shooting2, 1))

    plt.legend()
    plt.grid()
    plt.show()
