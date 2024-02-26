import numpy as np
import matplotlib.pyplot as plt
from imported.lab1_2 import threediagonal_solve


PI = np.pi
const_a =1
l = np.pi

x_begin = 0
x_end = l

t_begin = 0
t_end = 5

# Начально-краевая задача для ДУ параболического типа. [ур. теплопроводности]
def phi_0(t): # u_x(0, t)
    return np.exp(-const_a * t)

def phi_l(t): # u_x(pi, t)
    return -np.exp(-const_a * t)

def psi(x): # u(x, 0)
    return np.cos(x)

def exact_solution(x, t):
    return np.exp(-const_a * t) * np.cos(x)


# Неявная конечно-разностная схема для решения параболического ДУ
def solve_implicit(N, K):
    h = l / N
    tau = t_end / K
    sigma = tau * const_a ** 2 / h ** 2
    
    t = np.arange(0, t_end, tau)
    x = np.arange(0, x_end, h)
    T_SIZE, X_SIZE = len(t), len(x)

    u = np.zeros((T_SIZE, X_SIZE))
    u[0] = psi(x)    

    for k in range(1, T_SIZE):
        a = np.zeros(X_SIZE-2)
        b = np.zeros(X_SIZE-2)
        c = np.zeros(X_SIZE-2)
        
        #вычисления значений диагоналей
        a[:] = sigma
        b[:] = -(1 + 2 * sigma + 3 * tau ** 2)
        c[:] = sigma

        d = -u[k-1][1:-1]
        d[0] -= sigma * phi_0(t[k])
        d[-1] -= sigma * phi_l(t[k])
        
        u[k][0] = phi_0(t[k])
        u[k][-1] = phi_l(t[k])
        u[k][1:-1] = threediagonal_solve(a,b,c, d)

    return u


# Явная конечно-разностная схема для решения параболического ДУ
def solve_explicit(N, K):
    h = l / N
    tau = t_end / K
    sigma = tau * const_a ** 2 / h ** 2
  
    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)
    T_SIZE, X_SIZE = len(t), len(x)

    u = np.zeros((T_SIZE, X_SIZE))
    u[0] = psi(x)

    for k in range(1, T_SIZE): # временной шаг
        u[k][0] = phi_0(t[k])
        for j in range(1, X_SIZE-1): # пространственный шаг
            u[k][j] = sigma * (u[k - 1][j + 1] + u[k - 1][j - 1]) + (1 - 2 * sigma) * u[k - 1][j] # формула явной конечно-разностной схемы
        u[k][-1] = phi_l(t[k])

    return u

# Схема Кранка-Никольсона для решения параболического ДУ
def solve_crank_nicholson(N, K, approximation = 'two_point_first_order'):
    h = l / N
    tau = t_end / K
    sigma = tau * const_a ** 2 / h ** 2
    theta = 0.5
    
    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)
    T_SIZE, X_SIZE = len(t), len(x)
    
    u = np.zeros((T_SIZE, X_SIZE))
    u[0] = psi(x)    

    for k in range(1, T_SIZE):
        a = np.zeros(X_SIZE-2)
        b = np.zeros(X_SIZE-2)
        c = np.zeros(X_SIZE-2)

        #вычисления значений диагоналей
        a[:] = sigma*theta
        b[:] = -(1 + 2 * sigma*theta + 3 * tau ** 2)
        c[:] = sigma*theta

        d = np.array([-(u[k-1][i] +(1-theta) * sigma * (u[k-1][i-1] - 2*u[k-1][i] + u[k-1][i+1])) for i in range(1, X_SIZE-1)])

        d[0] -= sigma * theta * phi_0(t[k])
        d[-1] -= sigma * theta * phi_l(t[k])

        u[k][0] = phi_0(t[k])
        u[k][-1] = phi_l(t[k])
        u[k][1:-1] = threediagonal_solve(a, b, c, d)

    return u


# Аналитическое решение параболического ДУ
# возвращает сеточную функцию
def solve_exact_solution(N, K):    
    h = l / N
    tau = t_end / K
    
    t = np.arange(0, t_end, tau)
    x = np.arange(0, x_end, h)
    T_SIZE, X_SIZE = len(t), len(x)

    u = np.zeros((T_SIZE, X_SIZE))
    
    for k in range(T_SIZE): 
        for j in range(X_SIZE):
            u[k][j] = exact_solution(x[j], t[k])
    return u

def check_stability(sigma):
    assert sigma <= 0.5, sigma

def calc_mean_abs_error(numeric, analytic):
    return np.abs(numeric - analytic).mean(axis=1)


if __name__ == '__main__':
    N = 20
    K = 2000

    h = l / N
    tau = t_end / K
    sigma = tau * const_a ** 2 / h ** 2 # параметризация для устойчивости
    check_stability(sigma) # устойчивость важна для явной К-Р схемы
    
    appox_num = 0
    
    analitical_res = solve_exact_solution(N, K)
    implicit_res = solve_implicit(N, K)
    explicit_res = solve_explicit(N, K)
    crank_nicholson_res = solve_crank_nicholson(N, K)

    time = 10 
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 8))

    x = np.arange(0, x_end, h)
    t = np.arange(0, t_end, tau)
    labels = ['Exact', 'Explicit', 'Implicit', 'Crank Nicholson']

    ax1.set_title('U(x)')
    ax1.plot(x, analitical_res[time], color='g', label=labels[0])
    ax1.plot(x, explicit_res[time], color='y', label=labels[1])
    ax1.plot(x, implicit_res[time], color='b', label=labels[2])
    ax1.plot(x, crank_nicholson_res[time], color='brown', label=labels[3])
    ax1.grid()
    ax1.legend(loc='best')
    ax1.set_ylabel('U')
    ax1.set_xlabel('x')

    ax2.set_title('График ошибки')
    ax2.plot(t, calc_mean_abs_error(explicit_res, analitical_res), color='y', label=labels[1])
    ax2.plot(t, calc_mean_abs_error(implicit_res, analitical_res), color='b', label=labels[2])
    ax2.plot(t, calc_mean_abs_error(crank_nicholson_res, analitical_res), color='brown', label=labels[3])
    ax2.legend(loc='best')
    ax2.set_ylabel('Ошибка')
    ax2.set_xlabel('t')
    ax2.grid()

    plt.tight_layout()
    plt.show()
    

