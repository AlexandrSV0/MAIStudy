import math


def f(x):
    return math.acos(x) + x

# строит интерполяционный многочлен Лагранжа
def Lagrange_interpolation(x, y, x_checking):
    assert len(x) == len(y)
    
    polynom_str = 'L(x) ='
    polynom_result_value = 0  # = L(x*)
    
    for i in range(len(x)):
        i_polynom_part_srt = '' 
        i_checking_point = 1
        i_denom = 1

        for j in range(len(x)):
            if i == j:
                continue

            i_polynom_part_srt += f'(x-{x[j]:.2f})'
            i_checking_point *= (x_checking - x[j])
            i_denom *= (x[i] - x[j])

        polynom_str += f' + {(y[i] / i_denom):.2f}*' + i_polynom_part_srt
        polynom_result_value += y[i] * i_checking_point / i_denom # сумма значений полинома для x*

    return polynom_str, polynom_result_value

# строит интерполяционный многочлен Ньютона
def Newton_interpolation(x, y, x_checking):
    assert len(x) == len(y)
    # находим разделенные разности, коэфф-ты многочлена
    n = len(x)
    coefs = [y[i] for i in range(n)]

    for i in range(1, n):
        for j in range(n - 1, i - 1, -1):
            coefs[j] = float(coefs[j] - coefs[j - 1]) / float(x[j] - x[j - i])

    polynom_str = 'P(x) = '
    polynom_result_value = 0  # = P(x*)
    i_multiplies_str = ''
    i_multiplies = 1

    for i in range(n):
        polynom_result_value += i_multiplies * coefs[i]
        
        if i == 0:
            polynom_str += f'{coefs[0]:.2f}'
        else:
            polynom_str += ' + ' + i_multiplies_str + '*' + f'{coefs[i]:.2f}'

        i_multiplies *= (x_checking - x[i])
        i_multiplies_str += f'(x-{x[i]:.2f})'
        
    return polynom_str, polynom_result_value


if __name__ == '__main__':
    x_a, x_b = [-0.4, -0.1, 0.2, 0.5], [-0.4, 0, 0.2, 0.5]
    y_a, y_b = [f(x) for x in x_a], [f(x) for x in x_b]

    x_checking = 0.1
    polynom_expected_value = f(x_checking)

    lagrange_polynom_a, polynom_value_a = Lagrange_interpolation(x_a, y_a, x_checking)
    lagrange_polynom_b, polynom_value_b = Lagrange_interpolation(x_b, y_b, x_checking)

    print('------------------------')
    print('Интерполяционный многочлен Лагранжа')
    print('Полином A)')
    print(lagrange_polynom_a)
    print('Погрешность =', abs(polynom_value_a - polynom_expected_value))
    print('Полином B)')
    print(lagrange_polynom_b)
    print('Погрешность =', abs(polynom_value_b - polynom_expected_value))

    newton_polynom_a, polynom_value_a = Newton_interpolation(x_a, y_a, x_checking)
    newton_polynom_b, polynom_value_b = Newton_interpolation(x_b, y_b, x_checking)
    
    print('------------------------')
    print('Интерполяционный многочлен Ньютона')

    print('Полином A)')
    print(newton_polynom_a)
    print('Погрешность =', abs(polynom_value_a - polynom_expected_value))

    print('Полином B)')
    print(newton_polynom_b)
    print('Погрешность =', abs(polynom_value_b - polynom_expected_value))