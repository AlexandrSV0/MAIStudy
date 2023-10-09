import numpy as np

EPS = 0.00000000000001

# вычисляет знак числа
def sign(x):
    if x < 0:
        return -1
    if x > 0:
        return 1
    return 0

# вычисляет норму вектора как корень из скалярного квадрата
def L2_norm(vec):
    ans = 0
    for num in vec:
        ans += num * num
    
    return np.sqrt(ans)

# вычисляет матрицу хаусхолдера для итерации QR-разложения
def get_householder_matrix(A, col_num):
    n = A.shape[0]
    v = np.zeros(n)
    a = A[:, col_num]
    v[col_num] = a[col_num] + sign(a[col_num]) * L2_norm(a[col_num:])
    for i in range(col_num + 1, n):
        v[i] = a[i]
    v = v[:, np.newaxis] # преобразуем в столбец двумерный список
    H = np.eye(n) - (2 / (v.T @ v)) * (v @ v.T)
    return H

# выполняет QR разложение A = QR
def QR(A):
    n = A.shape[0]
    Q = np.eye(n)
    A_i = np.copy(A)

    for i in range(n - 1):
        # строим матрицу Хаусхолдера для столбца i и выполняем перемножение
        H = get_householder_matrix(A_i, i) 
        Q = Q @ H
        A_i = H @ A_i
        
    return Q, A_i

# вычисляет корни системы двух уравнений i и i+1 матрицы А
# a11 a12
# a21 a22
def get_roots(A, i):
    n = A.shape[0]
    a11 = A[i][i]
    a12 = A[i][i + 1] if i + 1 < n else 0
    a21 = A[i + 1][i] if i + 1 < n else 0
    a22 = A[i + 1][i + 1] if i + 1 < n else 0
    return np.roots((1, -a11 - a22, a11 * a22 - a12 * a21))

# Проверяем что i-е и i+1-е СЗ - комплексные
def is_complex(A, i):
    Q, R = QR(A)
    A_next = np.dot(R, Q)
    lambda1 = get_roots(A, i)
    lambda2 = get_roots(A_next, i)
    
    return abs(lambda1[0] - lambda2[0]) <= EPS and abs(lambda1[1] - lambda2[1]) <= EPS

# вычисляет i-ое (и i+1-е если комлпексное) СЗ матрицы А
def get_eigen_value(A, i):
    A_i = np.copy(A)
    
    while True:
        Q, R = QR(A_i)
        A_i = R @ Q
        
        # после преобразований собственные значения будут лежать на главной диагонали матрицы
        if L2_norm(A_i[i + 1:, i]) <= EPS:
            return A_i[i][i], A_i
        elif L2_norm(A_i[i + 2:, i]) <= EPS and is_complex(A_i, i):
            return get_roots(A_i, i), A_i

 
# вычисляет все СЗ матрицы A используя QR-разложение
def get_eigen_values_QR(A):
    n = A.shape[0]
    A_i = np.copy(A)
    eigen_values = []

    i = 0
    while i < n:
        cur_eigen_values, A_i_next = get_eigen_value(A_i, i)
        if isinstance(cur_eigen_values, np.ndarray):
            # комплексное
            eigen_values.extend(cur_eigen_values)
            i += 2
        else:
            # вещественное
            eigen_values.append(cur_eigen_values)
            i += 1
        A_i = A_i_next
        
    return eigen_values


if __name__ == '__main__':
    A = [
        [6, 5, -6],
        [4, -6, 9],
        [-6, 6, 1]
    ]
    A = np.array(A, dtype='float')
    # Q, R = QR(A);
    # print(Q@R)
    eig_values = get_eigen_values_QR(A)
    print('Собственные значения:', eig_values)