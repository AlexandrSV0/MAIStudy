from random import randrange
from math import gcd

def solovay_strassen(n, k):
	if n == 2 or n == 3:
		return True
	if not n & 1:
		return False
	
	def legendre(a, p):
		if (a == 0) or (a == 1):
			return a
		if a % 2 == 0:
			r = legendre(a // 2, p)
			if (p * p - 1) & 8 != 0:
				r *= -1
		else:
			r = legendre(p % a, a)
			if (a - 1) * (p - 1) & 4 != 0:
				r *= -1
			return r
		for i in range(k):
			a = randrange(2, n - 1)
			x = legendre(a, n)
			y = pow(a, (n - 1) // 2, n)
			if (x == 0) or (y != x % n):
				return False
		return True
to_factorize_value = 587001250609237122936576780197222991151753736674220109004474293607173917070470680797189229766409054553586774989887452450364068622528079086803614810202827588459315928046299951887979803334204273291981221032237387241947784071943078664650050662310962747619491223879940411727964302362450684574134527511533196962581
next_value =335761174067164167618543198964247689269563224294181999563165832133259540292187722805137445078814489162047464153361099572168058264117428507939423508728572440101286002782251421459113221509564615169373781381114169967893458821918147723049357233572020159694576243720955242412265730461627230303922059224402752942021
divider_1 = gcd(to_factorize_value, next_value)
divider_2 = to_factorize_value // divider_1
print("First divisor =", divider_1)
print("First divider is", '' if solovay_strassen(divider_1, 10000) else
"not", "prime")
print("Second divisor =", divider_2)
print("Second divider is", '' if solovay_strassen(divider_2, 10000) else
"not", "prime")
assert divider_1 * divider_2 == to_factorize_value