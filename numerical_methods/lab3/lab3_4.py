
def find_interval(x, x_checking):
    for i in range(len(x) - 1):
        if x[i] <= x_checking and x_checking < x[i + 1]:
            return i
    return -1
        

# вычисляет первую производную заданной функции в точке х*
def df(x, y, x_checking):
    assert len(x) == len(y)
    i = find_interval(x, x_checking)
    if i == -1 or i == len(x) - 2:
        i =  len(x) - 3

    add1 = (y[i+1] - y[i]) / (x[i+1] - x[i])
    add2 = ((y[i+2] - y[i+1]) / (x[i+2] - x[i+1]) - add1) / (x[i+2] - x[i]) * (2*x_checking - x[i] - x[i+1])
    return add1 + add2

# вычисляет вторую производную заданной функции в точке х*
def d2f(x, y, x_checking):
    assert len(x) == len(y)
    
    i = find_interval(x, x_checking)
    if i == -1 or i == len(x) - 2:
        i =  len(x) - 3
    
    tmp = (y[i+2] - y[i+1]) / (x[i+2] - x[i+1]) - (y[i+1] - y[i]) / (x[i+1] - x[i])
    return 2 * tmp / (x[i+2] - x[i])


if __name__ == '__main__':
    x = [-1.0, 0.0, 1.0, 2.0, 3.0]
    y = [1.3562, 1.5708, 1.7854, 2.4636, 3.3218]
    x_checking = 1.0

    print(f'df({x_checking}) = {df(x, y, x_checking)}')
    print(f'd2f({x_checking}) = {d2f(x, y, x_checking)}')
