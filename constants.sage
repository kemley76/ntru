import constants as c
P.<x> = PolynomialRing(ZZ)

phi_1 = P(x - 1)
#phi_n = P(x^c.n) / P(x - 1)
phi_n = sum([x^i for i in range(c.n)])
