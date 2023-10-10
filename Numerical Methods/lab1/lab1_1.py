import copy
import numpy as np

# LU-разложение - модификация метода Гаусса.
def LU(A):
    # L - нижне треугольная матрица
    # U - верхне треугольная матрица
    
    n = len(A)
    L = np.zeros((n,n))
    U = copy.deepcopy(A)
   
    for k in range(1, n):
        for i in range(k - 1, n):
            for j in range(i, n):
                L[j][i] = U[j][i] / U[i][i]

        for i in range(k, n):
            for j in range(k - 1, n):
                U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j]
    return L, np.array(U)


#решает систему уравнений Ax =  b с помощью LU-разложения
def solve_system(L, U, b):
    # Ly = b
    n = len(L)
    y = np.zeros(n)
    for i in range(n):
        sum = 0.
        for j in range(i):
            sum += L[i][j] * y[j]
        y[i] = (b[i] - sum) / L[i][i]
    
    # Ux = y 
    x = [0 for _ in range(n)]
    for i in range(n - 1, -1, -1):
        sum = 0
        for j in range(n - 1, i - 1, -1):
            sum += U[i][j] * x[j]
        x[i] = (y[i] - sum) / U[i][i]
    return x


def determinant(U):
    det = 1
    for i in range(len(U)):
        det *= U[i][i]
    return det

def inverse_matr(A):
    n = len(A)
    E = np.eye(n)
    L, U = LU(A)
    A_inversion = []
    for e in E:
        row_inversion = solve_system(L, U, e)
        A_inversion.append(row_inversion)
    return transpose_matr(np.array(A_inversion))


def transpose_matr(matr):
    n = len(matr)
    m = len(matr[0])
    matr_T = np.zeros((m, n))
    
    for i in range(n):
        for j in range(m):
            matr_T[j, i] = matr[i, j]
    return matr_T

if __name__ == '__main__':
    A = [
        [7, 8, 4, -6],
        [-1, 6, -2, -6],
        [2, 9, 6, -4],
        [5, 9, 1, 1]
    ]
    b = [-126, -42, -115, -67]
    
    L, U = LU(A)
    solution = solve_system(L, U, b)
    
    print("LU разложение:")
    print('L:')
    print(L)
    print('U:')
    print(U)
    
    print('----------------------')
    print("Решение системы")
    print('x:', solution)
    
    print('----------------------')
    print("det A =", determinant(U))
    print("Обратная матрица A:")
    print(inverse_matr(A))
    # print(inverse_matr(inverse_matr(A)))
    print(L@U)
