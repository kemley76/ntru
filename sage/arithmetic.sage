import constants

# From the original definitions:
Z.<x> = PolynomialRing(ZZ)
PHI_1 = Z.cyclotomic_polynomial(1)
PHI_N = Z.cyclotomic_polynomial(constants.n)
R = Z.quotient(PHI_1*PHI_N)
S = Z.quotient(PHI_N)

# Finds the non-normative representative of the given polynomial
# in the R/q quotient ring
# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
def Rq(a):
    ideal = R(a).lift()
    return Z([c % constants.q for c in ideal.list()])

# Finds the canonical representative of the given polynomial
# in the R/q quotient ring
# The difference is that the coefficients of the polynomial
# must be between (-q/2) and (q/2 - 1)
# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
def Rq_bar(a):
    ideal = R(a).lift()
    
    # Function that makes sure the coefficients of the
    # polynomial stay between (-q/2) and (q/2 - 1)
    def canon(c):
        c_mod = c % constants.q
        return int(c_mod - constants.q if c_mod >= constants.q / 2 else c_mod)
    
    return Z([canon(c) for c in ideal.list()])

# Finds the representative of the given polynomial in the 
# S/2 quotient ring
# Note: the non-normative and canonical representation in S/2
# are the same
# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(2,Φ_n))
def S2(a):
    ideal = S(a).lift()
    return Z([c % 2 for c in ideal.list()])

# Finds the non-normative representative of the given polynomial
# in the S/3 quotient ring
# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(3,Φ_n))
def S3(a):
    ideal = S(a).lift()
    return Z([c % 3 for c in ideal.list()])

# Finds the canonical representative of the given polynomial
# in the S/3 quotient ring
# The difference is that the coefficients of the polynomial
# must be between -1 and 1
# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(3,Φ_n))
def S3_bar(a):
    ideal = S(a).lift()

    # Function that makes sure the coefficients of the
    # polynomial stay between -1 and 1
    def canon(c):
        c_mod = c % 3
        return int(c_mod - 3 if c_mod > 1 else c_mod)
    
    return Z([canon(c) for c in ideal.list()])

# Finds the non-normative representative of the given polynomial
# in the S/q quotient ring
# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_n) that is not normative)
def Sq(a):
    ideal = S(a).lift()
    return Z([c % constants.q for c in ideal.list()])

# Finds the canonical representative of the given polynomial
# in the S/q quotient ring
# The difference is that the coefficients of the polynomial
# must be between (-q/2) and (q/2 - 1)
# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_n) that is canonical)
def Sq_bar(a):
    ideal = S(a).lift()

    def canon(c):
        c_mod = c % constants.q
        return int(c_mod - constants.q if c_mod >= constants.q / 2 else c_mod)
    
    return Z([canon(c) for c in ideal.list()])

# Compute inverses in S/2 quotient ring
# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(2,Φ_n))
def S2_inverse(a):
    R2.<x> = PolynomialRing(GF(2))
    Phi_n = cyclotomic_polynomial(constants.n)
    S2 = R2.quotient(Phi_n)

    in_poly = S2(a)

    b = ~in_poly

    return b

# Compute inverses in S/3 quotient ring
# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(3,Φ_n))
def S3_inverse(a):
    R3.<x> = PolynomialRing(GF(3))
    Phi_n = cyclotomic_polynomial(constants.n)
    S3 = R3.quotient(Phi_n)

    in_poly = S3(a)
    b = ~in_poly
    return b

# Compute inverses in S/q quotient ring
# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(q,Φ_n))
def Sq_inverse(a):
    v0 = S2(S2_inverse(a))
    t = 1
    stop = log2(constants.q)

    while t < stop:
        v0 = Sq(v0 * (2 - a * v0))
        t *= 2
    
    b = Sq(v0)

    # make sure that it follows the given paramters
    assert Sq(a*b) == 1, "Sq(a*b) wasn't one..."

    return b

# Maps the given polynomial over a small ternary ring
# Input: m (polynomial)
# Output: (Φ_1 * S3(m/Φ_1)) (polynomial) HRSS
def Lift(m):
    return (PHI_1 * S3_bar(m * S3_inverse(PHI_1)))
