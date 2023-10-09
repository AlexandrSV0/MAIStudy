import math
import numpy as np

EPS = 0.0000000000001
MAX_ITERS = 100000

def calc_q(a, b):
    return max(abs(dg(x)) for x in np.linspace(a,b,num = 100))

def f(x):
    return math.tan(x) - 5 * x * x  + 1

def df(x):
    return (1 / (math.cos(x) ** 2)) - 10 * x

def g(x):
    # поскольку стандартное преобразование к x = g(x) не позволяет получить корректное выполнение условия сходимости ( q < 1)
    # то представим уравнение в виде x = x - lambda*f(x)
    # для положительного корня lambda = -0.13
    # для отрицательного корня lambda = 0.13
    return x + 0.13 * f(x)
    # return (math.tan(x) + 1) / (5 * x)
    
def dg(x):
    return 1+ 0.13*df(x)
    # return (x - math.cos(x) * math.cos(x) * math.tan(x)) / (5 * x * x  * math.cos(x) * math.cos(x)) - 1 / (5 * x * x)

# вычисляет корень f(x) == 0 на интервале методом Итераций
# возвращает x и число итераций
def iteration_method(interval): 
    a, b = interval[0], interval[1] 
    q = calc_q(a,b)
    # print("q = ", q)
    x_prev = (b + a) * 0.5
    iters_count = 0
    
    while iters_count < MAX_ITERS:
        iters_count += 1
        x = g(x_prev)
        
        if q / (1 - q) * abs(x - x_prev) <= EPS:
            break

        x_prev = x
    return x, iters_count

# вычисляет корень уравнения f(x) == 0 на интервале методом Ньютона
# возвращает x и число итераций
def newton_method(interval,):
    a, b = interval[0], interval[1]
    x_prev = (a+b) * 0.5
    iters_count = 0
    
    while iters_count < MAX_ITERS:
        iters_count += 1
        x = x_prev - f(x_prev) / df(x_prev)

        if abs(x - x_prev) <= EPS:
            break
        x_prev = x
        
    return x, iters_count
    
def print_result(x, f):
    print(f"x = {x:.7f}    |    f(x) = {f(x):.10f}")

if __name__ == "__main__":
    l_iter, r_iter = 0.2, 1 # для lambda = -0.13
    # l_iter, r_iter = -1, -0.1 # для lambda = 0.13
    x_iter, i_iter = iteration_method((l_iter, r_iter))
    
    l_newton, r_newton = 0.3, 0.8 # для lambda = -0.13
    # l_newton, r_newton = -0.8, -0.3 # для lambda = 0.13
    x_newton, i_newton = newton_method((l_newton, r_newton))

    print('-------------')
    print('Метод итераций')
    print_result(x_iter, f)
    print('Итерации:', i_iter)

    print('-------------')
    print('Метод Ньютона')
    print_result(x_newton, f)
    print('Итерации:', i_newton)
