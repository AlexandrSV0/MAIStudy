import numpy as np
import matplotlib.pyplot as plt
from imported.lab1_2 import threediagonal_solve_2

x_begin = 0
x_end = np.pi

t_begin = 0
t_end = 5

# Метод к-р для решения ДУ гиперболического  типа [ур. процесса малых поперечных колебаний струны]
# граничные условия
def phi_0(t):
    return np.sin(2 * t)

def phi_1(t):
    return -np.sin(2 * t)

def psi_0(x):
    return 0

def psi_1(x):
    return 2 * np.cos(x)

def exact_solution(x, t):
    return np.cos(x) * np.sin(2 * t)


def solve_exact(h, tau,):
    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)

    u = np.zeros((len(t), len(x)))
    for idx in range(len(x)):
        for idt in range(len(t)):
            u[idt][idx] = exact_solution(x[idx], t[idt])

    return u

# Неявная конечно-разностная схема для решения параболического ДУ
def solve_implicit(h, tau, sigma):    
    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)
    u = np.zeros((len(t), len(x)))

    for j in range(len(x)):
        u[0][j] = psi_0(x[j])
        u[1][j] = psi_0(x[j]) + tau * psi_1(x[j])

    for k in range(2, len(t)):
        a = np.zeros(len(x)-2)
        b = np.zeros(len(x)-2)
        c = np.zeros(len(x)-2)
        d = np.zeros(len(x)-2)
        
        a[:] = sigma
        b[:] = -(1 + 2 * sigma + 3 * tau ** 2)
        c[:] = sigma

        d = -2 * u[k - 1][1:-1] + u[k - 2][1:-1]
        d[0] -= sigma * phi_0(t[k])
        d[-1] -= sigma * phi_1(t[k])
        
        u[k][0] = phi_0(t[k])
        u[k][-1] = phi_1(t[k])
        u[k][1:-1] = threediagonal_solve_2(a,b,c, d)

    return u


#  Явная конечно-разностная схема для решения гиперболического ДУ
def solve_explicit(h, tau, sigma):
    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)

    u = np.zeros((len(t), len(x)))
    for j in range(len(x)):
        u[0][j] = psi_0(x[j])
        u[1][j] = psi_0(x[j]) + tau * psi_1(x[j])

    for k in range(2, len(t)):
        u[k][0] = phi_0(t[k])
        for j in range(1, len(x) - 1):
            u[k][j] = (sigma * (u[k - 1][j + 1] - 2 * u[k - 1][j] + u[k - 1][j - 1]) + (2 - 3 * tau ** 2) * u[k - 1][j] - u[k - 2][j])
        u[k][-1] = phi_1(t[k])
    return u


def check_cond(sigma):
    assert sigma <= 0.5, sigma


def calc_mean_abs_error(numeric, analytic):
    return np.abs(numeric - analytic).mean(axis=1)

if __name__ == '__main__':
    N = 100 
    T = 2000

    h = x_end / N
    tau = t_end / T
    sigma = (tau / h) ** 2
    check_cond(sigma)
    
    explicit_res = solve_explicit(h, tau, sigma)
    implicit_res = solve_implicit(h, tau, sigma)
    analitical_res = solve_exact(h, tau)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 8))
    labels = ['Exact', 'Explicit', 'Implicit']
   
    time = 10
   
    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)

    ax1.set_title('U от x')
    ax1.plot(x, analitical_res[time], color='g', label=labels[0])
    ax1.plot(x, explicit_res[time], color='brown', label=labels[1])
    ax1.plot(x, implicit_res[time], color='b', label=labels[2])
    ax1.legend(loc='best')
    ax1.set_ylabel('U')
    ax1.set_xlabel('x')
    ax1.grid()
    
    ax2.set_title('График средней ошибки')
    ax2.plot(t, calc_mean_abs_error(explicit_res, analitical_res), color='brown',  label=labels[1])
    ax2.plot(t, calc_mean_abs_error(implicit_res, analitical_res), color='b',  label=labels[2])
    ax2.legend(loc='best')
    ax2.set_ylabel('Ошибка')
    ax2.set_xlabel('t')
    ax2.grid()
    
    plt.tight_layout()
    plt.show()