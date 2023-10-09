import numpy as np

def f(x):
    return (x**0.5) / (4 +  3*x) 

def calc_interval(x0, xk, h):
    return [i for i in np.arange(x0, xk + h, h)]

def calc_func(interval):
    return [f(x) for x in interval]


# вычисляет интеграл функции на интервале методом прямоугольников с шагом h
def integrate_by_rectangle(interval, h):
    return h * sum([f((interval[i] + interval[i-1]) / 2) for i in range(1, len(interval))])


# вычисляет интеграл функции на интервале методом трапеций с шагом h
def integrate_by_trapeze(func_values, h):
    return 0.5 * h * sum([func_values[i] + func_values[i-1] for i in range(1, len(func_values))])


# вычисляет интеграл функции на интервале методом Симпсона с шагом h
def integrate_by_simpson(func_values, h):
    return h/3  * (
                    func_values[0] + func_values[-1] + 
                    sum([func_values[i] * 2 for i in range(2, len(func_values) - 1, 2)]) + # четные индексы
                    sum([func_values[i] * 4 for i in range(1, len(func_values) - 1, 2)])    # нечетные индексы
                )

# вычисляет более точное значения интеграла с учетом погрешности предыдущих вычислений
def runge_romberg_method(h1, h2, integral_value1, integral_value2, p):
    k = h2 / h1
    return integral_value1 + (integral_value1 - integral_value2) / (k**p - 1)


def print_integrate_result(method_str, h1, h2, result_h1, result_h2):
    print('------------------')
    print(method_str)
    print(f"Шаг = {h1}. F = {result_h1}" )
    print(f"Шаг = {h2}. F = {result_h2}" )
    


if __name__ == '__main__':
    x0, xk = 1, 5 
    h1, h2 = 1.0, 0.5
    
    interval1 = calc_interval(x0, xk, h1)
    interval2 = calc_interval(x0, xk, h2)
    
    func_values1 = calc_func(interval1)
    func_values2 = calc_func(interval2)

    rectangle_result_h1 = integrate_by_rectangle(interval1, h1)
    rectangle_result_h2 = integrate_by_rectangle(interval2, h2)
    print_integrate_result('Метод прямоугольников', h1, h2, rectangle_result_h1, rectangle_result_h2)

    trapeze_result_h1 = integrate_by_trapeze(func_values1, h1)
    trapeze_result_h2 = integrate_by_trapeze(func_values2, h2)
    print_integrate_result('Метод трапеций', h1, h2, trapeze_result_h1, trapeze_result_h2)

    simpson_result_h1 = integrate_by_simpson(func_values1, h1)
    simpson_result_h2 = integrate_by_simpson(func_values2, h2)
    print_integrate_result('Метод Симпсона', h1, h2, simpson_result_h1, simpson_result_h2)


    rr_rectangle_res = runge_romberg_method(h1, h2, rectangle_result_h1, rectangle_result_h2, 3)
    rr_trapeze_res = runge_romberg_method(h1, h2, trapeze_result_h1, trapeze_result_h2, 3)
    rr_simpson_res = runge_romberg_method(h1, h2, simpson_result_h1, simpson_result_h2, 3)
    print('------------------')
    print('Метод Рунге-Ромберга:')
    print(f'* для метода прямоугольников = {rr_rectangle_res:.17}')
    print(f'* для метода трапеций        = {rr_trapeze_res:.17}')
    print(f'* для метода Симпсона        = {rr_simpson_res:.17}')
    
    print('------------------')
    print(f'Погрешность для метода прямоугольников, шаг {h1} = {rr_rectangle_res - rectangle_result_h1:.17}')
    print(f'Погрешность для метода прямоугольников, шаг {h2} = {rr_rectangle_res - rectangle_result_h2:.17}')
    print(f'Погрешность для метода трапеций, шаг {h1} = {rr_trapeze_res - trapeze_result_h1:.17}')
    print(f'Погрешность для метода трапеций, шаг {h2} = {rr_trapeze_res - trapeze_result_h2:.17}')
    print(f'Погрешность для метода Симпсона, шаг {h1} = {rr_simpson_res - simpson_result_h1:.17}')
    print(f'Погрешность для метода Симпсона, шаг {h2} = {rr_simpson_res - simpson_result_h2:.17}')
    