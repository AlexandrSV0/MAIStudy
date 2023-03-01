from geomdl import NURBS
from geomdl import utilities
from geomdl.visualization import VisMPL

# создание кривой
curve = NURBS.Curve()

#степень кривой
curve.degree = 4
# вектор контрольных точек (x, y)
curve.ctrlpts = [[1, 8], [3, 6], [5, 8], [8, 5], [10, 6]]
curve.weights = [1,9,6,4, 1]

# генерация вектора узлов
curve.knotvector = utilities.generate_knot_vector(curve.degree, len(curve.ctrlpts))

# "гладкость" кривой
curve.delta = 0.01

#отрисовка 2D-кривой
curve.vis = VisMPL.VisCurve2D()
curve.render()