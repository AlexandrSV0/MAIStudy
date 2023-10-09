import math
import numpy as np

from imported.lab1_1 import inverse_matr

EPS = 0.00000001
MAX_ITERS = 100000

def f1(X):
    return X[0] ** 2 - 2 * math.log10(X[1]) - 1

def f2(X):
    return X[0] ** 2 - 2 * X[0] * X[1]  + 2

def df1_dx1(X):
    return 2 * X[0]

def df1_dx2(X):
    return -2 * (1 / (X[1] * math.log(10)))

def df2_dx1(X):
    return 2 * X[0] - 2 * X[1]

def df2_dx2(X):
    return -2 * X[0]

def g1(X):
    # X1 = g1(X)
    return math.sqrt(2 * math.log10(X[1]) + 1)

def g2(X): # or X2 = 0
    # X2 = g2(X)
    return (X[0] ** 2 + 2) / (2 * X[0])

def dg1_dx1(X):
    return 0

def dg1_dx2(X):
    return 1 / (math.sqrt(math.log(10)) * X[1] * math.sqrt(2 * math.log(X[1]) + math.log(10)))

def dg2_dx1(X):
    return (X[0] ** 2 - 2) / (2 * X[0] ** 2)

def dg2_dx2(X):
    return 0

#норма L_inf = max(abs(a))
def L_inf_norm(a):
    return max([abs(x) for x in a])

#вычисляет q для метода итераций
def calc_q(interval1, interval2):
    a1, b1 = interval1
    a2, b2 = interval2
    x1 = (a1 + b1) / 2
    x2 = (a2 + b2) / 2
    
    return max(
            abs(dg1_dx1([x1, x2])) + abs(dg1_dx2([x1, x2])),       
            abs(dg2_dx1([x1, x2])) + abs(dg2_dx2([x1, x2]))
        )

# вычисляет решение системы нелинейных уравнений методом итераций 
def iteration_method(interval1, interval2):
    a1, b1 = interval1
    a2, b2 = interval2
    x_prev = [(a1 + b1) * 0.5, (a2 + b2) * 0.5]
    q = calc_q(interval1, interval2)
    iters_count = 0
    
    while iters_count < MAX_ITERS:
        iters_count += 1
        x = [g1(x_prev), g2(x_prev)]

        if q / (1 - q) * L_inf_norm([(x[i] - x_prev[i]) for i in range(len(x))]) < EPS:
            break
        
        x_prev = x
    return x, iters_count

# вычисляет матрицу Якоби в точке Х
def calc_jacobi_matr(x):
    jacobi = []
    jacobi.append([df1_dx1(x), df1_dx2(x)])
    jacobi.append([df2_dx1(x), df2_dx2(x)])
    return jacobi

#  вычисляет решение системы нелинейных уравнений методом Ньютона 
def newton_method(interval1, interval2):
    a1, b1 = interval1
    a2, b2 = interval2
    x_prev = [(a1 + b1) / 2, (a2 + b2) / 2]
    iters_count = 0
    
    while iters_count < MAX_ITERS:
        iters_count += 1
        jacobi_inversed = inverse_matr(calc_jacobi_matr(x_prev))
        #x_k+1 = x_k - J^(-1)(x_k) * f(x_k)
        x = x_prev - jacobi_inversed @ np.array([f1(x_prev), f2(x_prev)])

        if L_inf_norm([(x[i] - x_prev[i]) for i in range(len(x))]) < EPS:
            break
        x_prev = x
       
    return x, iters_count

def print_result(x, f1, f2):
    print(f"x = ({x[0]:.7f}, {x[1]:.7f})  |   f1(x) = {f1(x):.10f}    |   f2(x)= {f2(x):.10f}")


if __name__ == "__main__":
    a1, b1 =1, 1.5    
    a2, b2 = 1, 2
    
    x_iter, i_iter = iteration_method((a1, b1), (a2, b2))
    x_newton, i_newton = newton_method((a1, b1), (a2, b2))
    
    print('-------------')
    print('Метод итераций')
    print_result(x_iter, f1, f2)
    print('Итерации:', i_iter)
    
    print('-------------')
    print('Метод Ньютона')
    print_result(x_newton, f1, f2)
    print('Итерации:', i_newton)
    