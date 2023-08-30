import numpy as np


def tridiagonal_solve(A, b):
    # Ax=b, где A - трехдиагональная матрица
    # метод прогонки
    # прямой ход
    n = len(A)
    v = [0 for _ in range(n)]
    u = [0 for _ in range(n)]
    v[0] = A[0][1] / -A[0][0]
    u[0] = b[0] / A[0][0]
    
    for i in range(1, n-1):
        v[i] = A[i][i+1] / (-A[i][i] - A[i][i-1] * v[i-1])
        u[i] = (A[i][i-1] * u[i-1] - b[i]) / (-A[i][i] - A[i][i-1] * v[i-1])
         
    v[n-1] = 0
    u[n-1] = (A[n-1][n-2] * u[n-2] - b[n-1]) / (-A[n-1][n-1] - A[n-1][n-2] * v[n-2])

    # обратный ход
    x = [0 for _ in range(n)]
    x[n-1] = u[n-1]
    
    for i in range(n-1, 0, -1):
        x[i-1] = v[i-1] * x[i] + u[i-1]
    
    return x


if __name__ == "__main__":
    A = [
        [-6, 6, 0, 0, 0],
        [2, 10, -7, 0, 0],
        [0, -8, 18, 9, 0],
        [0, 0, 6, -17, -6],
        [0, 0, 0, 9, 14]
    ]
    
    b = [30, -31, 108, -114, 124]
    # b = [0, 5, 19, -17, 23]

    print('Решение методом прогонки')
    x = tridiagonal_solve(A, b)
    print(x)