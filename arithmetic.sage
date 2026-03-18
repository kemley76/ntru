import constants

# From the original definitions:
Z.<x> = PolynomialRing(ZZ)
PHI_1 = Z.cyclotomic_polynomial(1)
PHI_N = Z.cyclotomic_polynomial(constants.n)
R = Z.quotient(PHI_1*PHI_N)
S = Z.quotient(PHI_N)

# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n) that is not normative)
def Rq(a):
    ideal = R(a).lift()
    return Z([c % constants.q for c in ideal.list()])

# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n) that is canonical)
def Rq_bar(a):
    ideal = R(a).lift()
    
    def canon(c):
        c_mod = c % constants.q
        return int(c_mod - constants.q if c_mod >= constants.q / 2 else c_mod)
    
    return Z([canon(c) for c in ideal.list()])

# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(2,Φ_n) (both canonical and not normative))
def S2(a):
    P2.<x> = PolynomialRing(GF(2))
    Phi_n = P2.cyclotomic_polynomial(constants.n)
    S2 = P2.quotient(Phi_n)

    return S2(a)

# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(3,Φ_n) that is not normative)
def S3(a):
    ideal = S(a).lift()
    return Z([c % 3 for c in ideal.list()])

# input: a (polynomial in ring Z[x])
# output: b (polynomial in Z[x]/(3,Φ_n) that is canonical)
def S3_bar(a):
    ideal = S(a).lift()

    def canon(c):
        c_mod = c % 3
        return int(c_mod - 3 if c_mod > 1 else c_mod)
    
    return Z([canon(c) for c in ideal.list()])

# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_n) that is not normative)
def Sq(a):
    ideal = S(a).lift()
    return Z([c % constants.q for c in ideal.list()])

# input: a (polynomial in ring Z[x])
# output: b (polynomial in ring Z[x]/(q,Φ_n) that is canonical)
def Sq_bar(a):
    ideal = S(a).lift()

    def canon(c):
        c_mod = c % constants.q
        return int(c_mod - constants.q if c_mod >= constants.q / 2 else c_mod)
    
    return Z([canon(c) for c in ideal.list()])

# compute inverses in S/2
# (described here: http://cryptojedi.org/papers/#ntrukem)
# input: a (assuming a is a polynomial)
# output: b (polynomial)
def S2_inverse(a):
    R2.<x> = PolynomialRing(GF(2))
    Phi_n = cyclotomic_polynomial(constants.n)
    S2 = R2.quotient(Phi_n)

    in_poly = S2(a) #currently assuming a is a polynomial

    out_poly = ~in_poly

    # ... some other function that returns it in a certain form?
    b = out_poly

    return b

# compute inverses in S/3
# (described here: http://cryptojedi.org/papers/#ntrukem)
# input: a (binary lst? that represents the polynomial)
# output: b (binary lst? that represents the output)
def S3_inverse(a):
    R3.<x> = PolynomialRing(GF(3))
    Phi_n = cyclotomic_polynomial(constants.n)
    S3 = R3.quotient(Phi_n)

    in_poly = S3(a)
    out_poly = ~in_poly

    b = out_poly
    return b

# Input: a (polynomial)
# Output: b (polynomial)
def Sq_inverse(a):
    # unimplemented
    raise NotImplementedError("This function has not been implemented yet.")
    # should return a polynomial b (that satisfies Sq(a*b)=1)

# Input: m (polynomial)
# Output: (Φ_1 * S3(m/Φ_1)) (polynomial) HRSS
def Lift(m):
    # unimplemented
    raise NotImplementedError("This function has not been implemented yet.")
    # should return (Φ_1 * S3(m/Φ_1))