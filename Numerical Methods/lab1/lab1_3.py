import numpy as np

EPS = 0.000000001

def calc_norm(X):
    # ||X|| L1, где Х - матрица или вектор
    n = X.shape[0]
   
    if type(X[0]) == np.ndarray:
        l2_norm = abs(X[0][0])
        for i in range(n):
            for j in range(n):
                l2_norm = max(abs(X[i][j]), l2_norm)
    else:  #вектор
        l2_norm = abs(X[0])
        for i in range(n):
            l2_norm = max(abs(X[i]), l2_norm)

    return l2_norm


def solve_iterative(A, b):
    # Итеративный метод решения уравнения Ax=b
    n = A.shape[0]

    # 1. Ax=b -> x_k = alpha * x_(k-1) + beta
    alpha = np.zeros_like(A, dtype='float')
    beta = np.zeros_like(b, dtype='float')
    
    for i in range(n):
        for j in range(n):
            if i == j:
                alpha[i][j] = 0
            else:
                alpha[i][j] = -A[i][j] / A[i][i]

        beta[i] = b[i] / A[i][i]

    # 2. итерируем
    iterations = 0
    cur_x = np.copy(beta)
    converge = False
    
    while not converge and iterations < 10000:
        prev_x = np.copy(cur_x)
        cur_x = alpha @ prev_x + beta
        iterations += 1
        
        if calc_norm(alpha) < 1:
            converge = calc_norm(alpha) / (1 - calc_norm(alpha)) * calc_norm(cur_x - prev_x) <= EPS
        else:
            converge = calc_norm(cur_x - prev_x) <= EPS

    return cur_x, iterations


def seidel_multiplication(B, x, D):
    # B * x + D для метода Зейделя
    # x_(k+1) = D + Bx_(k+1) + Cx_(k) 
    res = np.copy(x)
    c = np.copy(B)
    
    for i in range(B.shape[0]):
        res[i] = D[i]
        for j in range(B.shape[1]):
            res[i] += B[i][j] * res[j]
            if j < i:
                c[i][j] = 0

    return res, c


def solve_seidel(A, b):
    # Метод Зейделя для решения уравнения Ax=b
    # является модификацией метода простых итераций
    n = A.shape[0]

    # 1. Ax=b -> x = alpha * x + beta
    alpha = np.zeros_like(A, dtype='float')
    beta = np.zeros_like(b, dtype='float')
    
    for i in range(n):
        for j in range(n):
            if i == j:
                alpha[i][j] = 0
            else:
                alpha[i][j] = -A[i][j] / A[i][i]

        beta[i] = b[i] / A[i][i]

    # 2. итерируем
    iterations = 0
    cur_x = np.copy(beta)
    converge = False
   
    while not converge:
        prev_x = np.copy(cur_x)
        cur_x, c = seidel_multiplication(alpha, prev_x, beta)
        iterations += 1
        
        if calc_norm(alpha) < 1:
            converge = calc_norm(c) / (1 - calc_norm(alpha)) * calc_norm(cur_x - prev_x) <= EPS
        else:
            converge = calc_norm(prev_x - cur_x) <= EPS

    return cur_x, iterations


if __name__ == '__main__':
    A = [
        [10, -1, -2, 5],
        [4, 28, 7, 9],
        [6, 5, -23, 4],
        [1, 4, 5, -15]
    ]
    A = np.array(A, dtype='float')
    b = [-99, 0, 67, 58]
    
    solution_yacobi, iters_yacobi = solve_iterative(A, b)
    solution_seidel, iters_seidel = solve_seidel(A, b)

    print('-------------')
    print('Метод простых итераций')
    print(solution_yacobi)
    print('Итерации:', iters_yacobi)
    
    print('-------------')
    print('Метод Зейделя')
    print(solution_seidel)
    print('Итерации:', iters_seidel)
