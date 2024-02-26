import numpy as np
import copy
from matplotlib import pylab
from matplotlib import cm
from imported.lab1_2 import threediagonal_solve
import matplotlib.pyplot as plt

a = 1

x_begin = 0
x_end = np.pi

y_begin = 0
y_end = np.pi

t_begin = 0
t_end = 1

Lx = np.pi
Ly = np.pi

M1 = 1
M2 = 1

# Решение многомерных задач мат. физики методами к-р. Методы расщепления

def phi1(y, t):
    return np.cos(M2 * y) * np.exp(-(M1 ** 2 + M2 ** 2) * a * t)

def phi2(y, t):
    return (-1) ** M1 * np.cos(M2 * y) * np.exp(-(M1 ** 2 + M2 ** 2) * a * t)

def phi3(x, t):
    return np.cos(M1 * x) * np.exp(-(M1 ** 2 + M2 ** 2) * a * t)

def phi4(x, t):
    return (-1) ** M2 * np.cos(M1 * x) * np.exp(-(M1 ** 2 + M2 ** 2) * a * t)

def psi(x, y):
    return np.cos(M1 * x) * np.cos(M2 * y)

def exact_solution(x, y, t):
    return np.cos(M1 * x) * np.cos(M2 * y) * np.exp(-(M1 ** 2 + M2 ** 2) * a * t)

hx = 0.09
hy = 0.09

tau = 0.01

# Метод переменных направлений
# использует неявную схему по одному направлению и явную по другому
def solve_variable_dirs():
    x = np.arange(0, x_end, hx)
    y = np.arange(0, y_end, hy)
    t = np.arange(0, t_end, tau)

    res = np.zeros((len(t), len(x), len(y)))

    # начальные значения
    for x_id in range(len(x)):
        for y_id in range(len(y)):
            res[0][x_id][y_id] = psi(x[x_id], y[y_id])

    for t_id in range(1, len(t)):
        U_halftime = np.zeros((len(x), len(y)))

        # краевые значения по x и по y
        for x_id in range(len(x)):
            res[t_id][x_id][0] = phi3(x[x_id], t[t_id])
            res[t_id][x_id][-1] = phi4(x[x_id], t[t_id])
            U_halftime[x_id][0] = phi3(x[x_id], t[t_id] - tau / 2)
            U_halftime[x_id][-1] = phi4(x[x_id], t[t_id] - tau / 2)

        for y_id in range(len(y)):
            res[t_id][0][y_id] = phi1(y[y_id], t[t_id])
            res[t_id][-1][y_id] = phi2(y[y_id], t[t_id])
            U_halftime[0][y_id] = phi1(y[y_id], t[t_id] - tau / 2)
            U_halftime[-1][y_id] = phi2(y[y_id], t[t_id] - tau / 2)

        # 1 Решение системы по переменной х. В результате получается промежуточное решение (u_halftime), которое используется для решения по y.
        for y_id in range(1, len(y) - 1):
            A = np.zeros((len(x) - 2, len(x) - 2))
            b = np.zeros((len(x) - 2))

            A[0][0] = 2 * hx ** 2 * hy ** 2 + 2 * a * tau * hy ** 2
            A[0][1] = -a * tau * hy ** 2
            for i in range(1, len(A) - 1):
                A[i][i - 1] = -a * tau * hy ** 2
                A[i][i] = 2 * hx ** 2 * hy ** 2 + 2 * a * tau * hy ** 2
                A[i][i + 1] = -a * tau * hy ** 2
            A[-1][-2] = -a * tau * hy ** 2
            A[-1][-1] = 2 * hx ** 2 * hy ** 2 + 2 * a * tau * hy ** 2

            for x_id in range(1, len(x) - 1):
                b[x_id - 1] = (
                        res[t_id - 1][x_id][y_id - 1] * a * tau * hx ** 2
                        + res[t_id - 1][x_id][y_id] * (2 * hx ** 2 * hy ** 2 - 2 * a * tau * hx ** 2)
                        + res[t_id - 1][x_id][y_id + 1] * a * tau * hx ** 2
                )
            b[0] -= (-a * tau * hy ** 2) * phi1(y[y_id], t[t_id] - tau / 2)
            b[-1] -= (-a * tau * hy ** 2) * phi2(y[y_id], t[t_id] - tau / 2)
            U_halftime[1:-1, y_id] = np.array(threediagonal_solve(A, b))

        # 2. Решение системы по переменной y. 
        for x_id in range(1, len(x) - 1):
            A = np.zeros((len(y) - 2, len(y) - 2))
            b = np.zeros((len(y) - 2))

            A[0][0] = 2 * hx ** 2 * hy ** 2 + 2 * a * tau * hx ** 2
            A[0][1] = -a * tau * hx ** 2
            for i in range(1, len(A) - 1):
                A[i][i - 1] = -a * tau * hx ** 2
                A[i][i] = 2 * hx ** 2 * hy ** 2 + 2 * a * tau * hx ** 2
                A[i][i + 1] = -a * tau * hx ** 2
            A[-1][-2] = -a * tau * hx ** 2
            A[-1][-1] = 2 * hx ** 2 * hy ** 2 + 2 * a * tau * hx ** 2

            for y_id in range(1, len(y) - 1):
                b[y_id - 1] = (
                        U_halftime[x_id - 1][y_id] * a * tau * hy ** 2
                        + U_halftime[x_id][y_id] * (2 * hx ** 2 * hy ** 2 - 2 * a * tau * hy ** 2)
                        + U_halftime[x_id + 1][y_id] * a * tau * hy ** 2
                )
            b[0] -= (-a * tau * hx ** 2) * phi3(x[x_id], t[t_id])
            b[-1] -= (-a * tau * hx ** 2) * phi4(x[x_id], t[t_id])
            # окончательный результат для текущей итерации             
            res[t_id][x_id][1:-1] = threediagonal_solve(A, b)
    return res

# Метод дробных шагов
# использует явные схемы в обоих направлениях
def solve_fractional_steps():
    x = np.arange(0, x_end, hx)
    y = np.arange(0, y_end, hy)
    t = np.arange(0, t_end, tau)
    
    res = np.zeros((len(t), len(x), len(y)))

    # начальные значения
    for x_id in range(len(x)):
        for y_id in range(len(y)):
            res[0][x_id][y_id] = psi(x[x_id], y[y_id])

    for t_id in range(1, len(t)):
        U_halftime = np.zeros((len(x), len(y)))

        # вычисление краевых значений по y и по х
        for x_id in range(len(x)):
            res[t_id][x_id][0] = phi3(x[x_id], t[t_id])
            res[t_id][x_id][-1] = phi4(x[x_id], t[t_id])
            U_halftime[x_id][0] = phi3(x[x_id], t[t_id] - tau / 2)
            U_halftime[x_id][-1] = phi4(x[x_id], t[t_id] - tau / 2)

        for y_id in range(len(y)):
            res[t_id][0][y_id] = phi1(y[y_id], t[t_id])
            res[t_id][-1][y_id] = phi2(y[y_id], t[t_id])
            U_halftime[0][y_id] = phi1(y[y_id], t[t_id] - tau / 2)
            U_halftime[-1][y_id] = phi2(y[y_id], t[t_id] - tau / 2)

        # 1. Решение системы относительно x. Получаем промежуточное значение u_halftime
        for y_id in range(1, len(y) - 1):
            A = np.zeros((len(x) - 2, len(x) - 2))
            b = np.zeros((len(x) - 2))

            A[0][0] = hx ** 2 + 2 * a * tau
            A[0][1] = -a * tau
            for i in range(1, len(A) - 1):
                A[i][i - 1] = -a * tau
                A[i][i] = hx ** 2 + 2 * a * tau
                A[i][i + 1] = -a * tau
            A[-1][-2] = -a * tau
            A[-1][-1] = hx ** 2 + 2 * a * tau

            for x_id in range(1, len(x) - 1):
                b[x_id - 1] = res[t_id - 1][x_id][y_id] * hx ** 2
            b[0] -= (-a * tau) * phi1(y[y_id], t[t_id] - tau / 2)
            b[-1] -= (-a * tau) * phi2(y[y_id], t[t_id] - tau / 2)
            U_halftime[1:-1, y_id] = np.array(threediagonal_solve(A, b))

        # 2. Решение системы относительно y. Вычисляем финальные значения на основе подсчитанных на предыдущем шаге u_halftime
        for x_id in range(1, len(x) - 1):
            A = np.zeros((len(y) - 2, len(y) - 2))
            b = np.zeros((len(y) - 2))

            A[0][0] = hy ** 2 + 2 * a * tau
            A[0][1] = -a * tau
            for i in range(1, len(A) - 1):
                A[i][i - 1] = -a * tau
                A[i][i] = hy ** 2 + 2 * a * tau
                A[i][i + 1] = -a * tau
            A[-1][-2] = -a * tau
            A[-1][-1] = hy ** 2 + 2 * a * tau

            for y_id in range(1, len(y) - 1):
                b[y_id - 1] = U_halftime[x_id][y_id] * hy ** 2
            b[0] -= (-a * tau) * phi3(x[x_id], t[t_id])
            b[-1] -= (-a * tau) * phi4(x[x_id], t[t_id])
            res[t_id][x_id][1:-1] = threediagonal_solve(A, b)
    return res


def solve_exact(x, y, t):
    u = np.zeros((len(t), len(x), len(y)))
    for idx in range(len(x)):
        for idy in range(len(y)):
            for idt in range(len(t)):
                u[idt][idx][idy] = exact_solution(x[idx], y[idy], t[idt])
    return u

def calc_max_abs_error(numeric, analytic):
    return np.abs(analytic- numeric).max()


if __name__ == '__main__':    
    x = np.arange(0, x_end, hx)
    y = np.arange(0, y_end, hy)
    t = np.arange(0, t_end, tau)
    xgrid, ygrid = np.meshgrid(x, y)

    analitic_grid = solve_exact(x, y, t)

    methods = {
        'Variable directions method': solve_variable_dirs,
        "Fractional steps method": solve_fractional_steps,
    }
    for _, (method_name, solve_method) in enumerate(methods.items()):
        time_step = 0
        CUR_Y = 0
        zgrid = solve_method()
        plt.figure(figsize=(9, 9))
        plt.grid()
        plt.plot(x, zgrid[time_step][:, CUR_Y], label=method_name)
        plt.title(method_name)
        # fig = pylab.figure(figsize=(12, 4))
        # ax = fig.add_subplot(1, 3, 1, projection='3d')
        # ax.plot_surface(xgrid, ygrid, zgrid[time_step].transpose(), rstride=1, cstride=1, cmap=cm.jet)
        # ax.set_title(method_name)

        # ax = fig.add_subplot(1, 3, 2, projection='3d')
        # ax.plot_surface(xgrid, ygrid, analitic_grid[time_step].transpose(),
        #                 rstride=1, cstride=1, cmap=cm.jet)
        # ax.set_title("Exact solution")

        # ax = fig.add_subplot(1, 3, 3, projection='3d')
        # ax.plot_surface(xgrid, ygrid, abs(analitic_grid - zgrid)[time_step].transpose(),
        #                 rstride=1, cstride=1, cmap=cm.jet)
        # ax.set_title("Error")

        # график ошибки от времени
        plt.figure(figsize=(6, 6))
        max_abs_errors = np.array([
            calc_max_abs_error(analitic_grid[i], zgrid[i])
            for i in range(len(t))
        ])
        plt.plot(t, max_abs_errors, label=method_name)
        plt.grid()
        plt.xlabel('time')
        plt.title('Max abs error from time.  ' + method_name)
        # plt.legend()
    pylab.show()
    plt.show()

