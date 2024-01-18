import numpy as np

# метод подсчета средней квадратичной ошибки
def mean_squared_error(y, y_correct):
    return np.sqrt(np.sum((y - y_correct)**2))

# метод прогонки решения уравнения Ax = b
# где A - трехдиагональная матрица
def tridiagonal_solve(A, b):
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


# метод конечных разностей для решения краевой задачи нелинейного ДУ 2 порядка
def finite_differrence(cond1, cond2, equation, borders, h=0.01, accuracy=2):
    x = np.arange(borders[0], borders[1] + h, h)
    N = np.shape(x)[0]

    A = np.zeros((N, N))
    b = np.zeros(N)

    for i in range(1, N - 1):
        A[i][i-1] = 1/h**2 - equation['p'](x[i])/(2*h)
        A[i][i] = -2/h**2 + equation['q'](x[i])
        A[i][i+1] = 1/h**2 + equation['p'](x[i])/(2*h)
        b[i] = equation['f'](x[i])

    #аппроксимация 1 порядка
    if accuracy == 1:
        A[0][0] = cond1['a'] - cond1['b']/h
        A[0][1] = cond1['b']/h
        b[0] = cond1['c']

        A[N-1][N-2] = -cond2['b']/h
        A[N-1][N-1] = cond2['a'] + cond2['b']/h
        b[N-1] = cond2['c']

    #аппроксимация 2 порядка
    elif accuracy == 2:
        p = equation['p']
        q = equation['q']
        a1 = cond1['a']; b1 = cond1['b']; c1 = cond1['c']
        a2 = cond2['a']; b2 = cond2['b']; c2 = cond2['c']

        A[0][0] = a1 - (3*b1)/(2*h) + ((2 - h*p(x[1]))*b1)/((2 + h*p(x[1]))*2*h)
        A[0][1] = 2*b1/h + ((h*h*q(x[1]) - 2)*b1)/((2 + h*p(x[1]))*h)
        A[0][2] = 0
        b[0] = c1
        
        A[N-1][N-3] = 0
        A[N-1][N-2] = -2*b2/h - ((h*h*q(x[N-2]) - 2)*b2)/((2 - h*p(x[N-2]))*h)
        A[N-1][N-1] = a2 + 3*b2/(2*h) - ((2 + h*p(x[N-2]))*b2)/((2 - h*p(x[N-2]))*2*h)
        b[N-1] = c2
    
    return tridiagonal_solve(A, b)
