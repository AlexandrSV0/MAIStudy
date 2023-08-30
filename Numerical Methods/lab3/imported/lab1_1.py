import copy
import numpy as np
def LU_decomposition(A):
    # LU_decomposition-разложение: A = L*U, где:
    # L - нижне треугольная матрица
    # U - верхне треугольная матрица
    # LU_decomposition-разложение - модификация метода Гаусса.
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
    return L, U


def LU_solve_system(L, U, b):
    #Ly = b
    n = len(L)
    y = np.zeros(n)
    for i in range(n):
        sum = 0.
        for j in range(i):
            sum += L[i][j] * y[j]
        y[i] = (b[i] - sum) / L[i][i]
    
    #Ux = y 
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

def inverse_matrix(A):
    n = len(A)
    E = np.eye(n)
    L, U = LU_decomposition(A)
    A_inversion = []
    for e in E:
        row_inversion = LU_solve_system(L, U, e)
        A_inversion.append(row_inversion)
    return transpose(np.array(A_inversion))


def transpose(matrix):
    n = len(matrix)
    m = len(matrix[0])
    transposed_matrix = np.zeros((m, n))
    for i in range(n):
        for j in range(m):
            transposed_matrix[j, i] = matrix[i, j]
    return transposed_matrix

def print_matrix(A):
    n = len(A)
    m = len(A[0])
    for i in range(n):
        for j in range(m):
            print(f'%6.2f' % A[i][j], end=' ')
        print()

if __name__ == '__main__':
    A = [
        [7, 8, 4, -6],
        [-1, 6, -2, -6],
        [2, 9, 6, -4],
        [5, 9, 1, 1]
    ]
    b = [-126, -42, -115, -67]
    
    print("LU_decomposition разложение:")
    L, U = LU_decomposition(A)
    print('L:')
    print_matrix(L)
    print('U:')
    print_matrix(U)
    print("Решение системы")
    solution = LU_solve_system(L, U, b)
    print('x:', solution)
    print("det A =", determinant(U))
    print("Обратная матрица A:")
    print_matrix(inverse_matrix(A))

