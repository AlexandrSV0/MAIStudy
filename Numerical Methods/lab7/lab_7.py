import numpy as np
import copy
from matplotlib import pylab
from matplotlib import cm
import matplotlib.pyplot as plt

ITERS_MAX = 10000
EPSILON = 1e-6


# Решение ДУ эллиптического типа методом к-р. [ур. Пуассона]
A = 0
B = 0
C = 0
D = 1

Lx = np.pi
Ly = 1

alpha1, beta1 = 1, 0
gamma1, delta1 = 1, 0

alpha2, beta2 =0, 1
gamma2, delta2 =0, 1

def phi1(y):
    return np.exp(y)

def phi2(y):
    return -np.exp(y)

def phi3(x):
    return np.sin(x)

def phi4(x):
    return np.e * np.sin(x)

def exact_solution(x,y):
    return np.sin(x) * np.exp(y)

def Function(x,y):  
    return 0


def calc_accuracy(u, u_prev):
    res = 0
    for i in range(Nx):
        for j in range(Ny):
            res = max(res, abs(u[i][j] - u_prev[i][j]))
    return res

def init_boundary_values(u, x, y):
    u[0, :] = phi1(y)
    u[-1, :] = phi2(y)
    u[:, 0] = phi3(x)
    u[:, -1] = phi4(x)
    
# Метод простых итераций (метод Либмана)
def solve_simple_iteration(Nx, Ny, hx, hy):
    hx = Lx / Nx
    hy = Ly / Ny
    x = np.linspace(0, Lx, Nx)
    y = np.linspace(0, Ly, Ny)
    u = np.zeros((Nx, Ny))

    init_boundary_values(u, x, y)

    for iteration in range(ITERS_MAX):
        u_prev = copy.deepcopy(u)
        # граничные условия
        u[0, 1:Ny - 1] = u[1, 1:Ny - 1] - hx * phi1(y[1:Ny - 1])
        u[-1, 1:Ny - 1] = u[-2, 1:Ny - 1] + hx * phi2(y[1:Ny - 1])
        
        # Центрально-разностная аппроксимация
        for i in range(1, Nx - 1):
            for j in range(1, Ny - 1):
                u[i][j] = (hx ** 2 * Function(x[i], y[j]) - (
                            u_prev[i + 1][j] + u_prev[i - 1][j]) - D * hx ** 2 * (
                             u_prev[i][j + 1] + u_prev[i][j - 1]) / (hy ** 2) - A * hx * 0.5 * (
                             u_prev[i + 1][j] - u_prev[i - 1][j]) - B * hx ** 2 * (
                             u_prev[i][j + 1] - u_prev[i][j - 1]) / (2 * hy)) / (
                            C * hx ** 2 - 2 * (hy ** 2 + D * hx ** 2) / (hy ** 2))

        if calc_accuracy(u, u_prev) <= EPSILON:
            return u.transpose(), iteration+1
    else:
        print("Решение не сошлось")

   
def solve_Zeidel(Nx, Ny, hx, hy):
    hx = Lx / Nx
    hy = Ly / Ny
    x = np.linspace(0, Lx, Nx)
    y = np.linspace(0, Ly, Ny)
    u = np.zeros((Nx, Ny))

    init_boundary_values(u, x, y)

    for iteration in range(ITERS_MAX):
        u_prev = copy.deepcopy(u)
        # граничные условия
        u[0, 1:Ny - 1] = u[1, 1:Ny - 1] - hx * phi1(y[1:Ny - 1])
        u[-1, 1:Ny - 1] = u[-2, 1:Ny - 1] + hx * phi2(y[1:Ny - 1])

        calc_universal_Zeidel_step(u, u_prev, x, y, hx, hy)                   
                            
        if calc_accuracy(u, u_prev) <= EPSILON:
            return u.transpose(), iteration+1
    else:
        print("Решение не сошлось")

def calc_universal_Zeidel_step(u, u_prev, x, y, hx, hy, relax=False, paramRelax = 1):
    for i in range(1, len(u) - 1):
            for j in range(1, len(u[0]) - 1):
                M = (hx ** 2 * Function(x[i], y[j]) - (
                            u_prev[i + 1][j] + u[i - 1][j]) - D * hx ** 2 * (
                            u_prev[i][j + 1] + u[i][j - 1]) / (hy ** 2) - A * hx * 0.5 * (
                            u_prev[i + 1][j] - u[i - 1][j]) - B * hx ** 2 * (
                            u_prev[i][j + 1] - u[i][j - 1]) / (2 * hy)) / (
                            C * hx ** 2 - 2 * (hy ** 2 + D * hx ** 2) / (hy ** 2))       
                if relax:
                    u[i][j] = (1 - paramRelax) * u_prev[i][j] + paramRelax * M
                else:
                    u[i][j] = M

def solve_simple_iteration_relaxed(Nx, Ny, hx, hy):
    hx = Lx / Nx
    hy = Ly / Ny
    x = np.linspace(0, Lx, Nx)
    y = np.linspace(0, Ly, Ny)
    u = np.zeros((Nx, Ny))

    init_boundary_values(u, x, y)

    paramRelax = 1.5
    for iteration in range(ITERS_MAX):
        u_prev = copy.deepcopy(u)
        # граничные условия
        u[0, 1:Ny - 1] = u[1, 1:Ny - 1] - hx * phi1(y[1:Ny - 1])
        u[-1, 1:Ny - 1] = u[-2, 1:Ny - 1] + hx * phi2(y[1:Ny - 1])

        calc_universal_Zeidel_step(u, u_prev, x, y, hx, hy, relax=True, paramRelax=paramRelax)


        if calc_accuracy(u, u_prev) <= EPSILON:
            return u.transpose(), iteration+1
    else:
        print("Решение не сошлось")


def solve_exact():
    x = np.linspace(0, Lx, Nx)
    y = np.linspace(0, Ly, Ny)
    u = []
    for yi in y:
        u.append([exact_solution(xi, yi) for xi in x])
    return u


def calc_abs_error(numeric, analytic):
    return np.abs(numeric - analytic)

def calc_max_abs_error(numeric, analytic):
    return np.abs(numeric - analytic).max()


if __name__ == '__main__':
    Nx = 14
    Ny = 14
    hx = Lx / Nx
    hy = Ly / Ny
    
    x = np.linspace(0, Lx, Nx)
    y = np.linspace(0, Ly, Ny)

    xgrid, ygrid = np.meshgrid(x, y)

    analitic_grid = np.array(solve_exact())
    methods = {
        'Simple Iteration method': solve_simple_iteration,
        "Zeidel's method": solve_Zeidel,
        'Relaxation method': solve_simple_iteration_relaxed
    }
    for _, (method_name, solve_method) in enumerate(methods.items()):
        CUR_Y = 0
        zgrid, iters = solve_method(Nx, Ny, hx, hy)
        plt.figure(figsize=(9, 9))
        plt.grid()
        plt.plot(x, zgrid[:, CUR_Y], label=method_name)
        plt.title(method_name)
        
        # fig = pylab.figure(figsize=(12, 4))
        # ax = fig.add_subplot(1, 3, 1, projection='3d')
        # ax.plot_surface(xgrid, ygrid, zgrid, rstride=1, cstride=1, cmap=cm.jet)
        # ax.set_title(method_name)

        # ax = fig.add_subplot(1, 3, 2, projection='3d')
        # ax.plot_surface(xgrid, ygrid, analitic_grid, rstride=1, cstride=1, cmap=cm.jet)
        # ax.set_title("Exact solution")

        # ax = fig.add_subplot(1, 3, 3, projection='3d')
        # ax.plot_surface(xgrid, ygrid, calc_abs_error(analitic_grid, zgrid), rstride=1, cstride=1, cmap=cm.jet)
        # ax.set_title("Error")
        print(f'{method_name} : {iters} iters')
        
        plt.figure(figsize=(7, 7))        
        max_abs_errors = np.array([
            calc_max_abs_error(analitic_grid[:, i], zgrid[:, i])
            for i in range(Nx)
        ])
        plt.plot(y, max_abs_errors, label=method_name) 
        plt.xlabel('y')
        plt.title('Max abs error ' + method_name)
    pylab.show()
    plt.show()




