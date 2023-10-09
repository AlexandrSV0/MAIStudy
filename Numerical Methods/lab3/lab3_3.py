import matplotlib.pyplot as plt

from imported.lab1_1 import LU, solve_system

# сумма квадратов ошибок
def sum_squared_errors(x, y, coefs):
    F_x = [F(coefs, x_i) for x_i in x]
    return sum((zipped[0] - zipped[1])**2 for zipped in zip(y, F_x))

# МНК. вычисляет приближающий многочлен степени n для заданной функции
def mls(x, y, n):
    A, b = [], []
    
    for k in range(n + 1):
        A.append([sum(x_i ** (i+k) for x_i in x) for i in range(n + 1)]) # SUM_i (x_i ^{k+i})
        b.append(sum([zipped[0] * zipped[1]**k for zipped in zip(y, x)])) # SUM_j (y_j * x _j ^ k)

    L, U = LU(A)
    return solve_system(L, U, b) # возвращает список коэф-ов a_i
    
# значение приближающего многочлена для конкретного Х
def F(coefs, x):
    return sum([x ** i * coefs[i] for i in range(len(coefs))])


if __name__ == '__main__':
    x = [-0.7, -0.4, -0.1, 0.2, 0.5, 0.8]
    y = [1.6462, 1.5823, 1.571, 1.5694, 1.5472, 1.4435]
    plt.scatter(x, y, color='b')
    plt.plot(x, y, color='b', linestyle='dotted', label='исходные данные')
    

    mls_result1 = mls(x, y, 1)
    mls_result2 = mls(x, y, 2)

    print('---------------------')
    print('Метод наименьших квадратов.')
    print('n = 1')
    print(f'F(x) = {mls_result1[0]} + {mls_result1[1]}x')
    print(f'Сумма квадратов ошибок = {sum_squared_errors(x, y, mls_result1)}')
    plt.plot(x, [F(mls_result1, x_i) for x_i in x], color='r', label='n = 1')

    print('---------------------')
    print('Метод наименьших квадратов.')
    print('n = 2')
    print(f'F(x) = {mls_result2[0]} + {mls_result2[1]}x + {mls_result2[2]}x^2')
    print(f'Сумма квадратов ошибок = {sum_squared_errors(x, y, mls_result2)}')
    plt.plot(x, [F(mls_result2, x_i) for x_i in x], color='y', label='n = 2')
     
    plt.legend()
    plt.grid()
    plt.show()
