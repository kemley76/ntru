load('constants.sage')

def S3(a):
    s3_mapping = [0, 1, -1] # 0 -> 0, 1 -> 1, 2 -> -1
    assert a in P
    intermediate = a % phi_n
    coeffs = intermediate.list()
    result = P(sum([x^i * s3_mapping[a % 3] for (i, a) in enumerate(coeffs)]))
    assert result.degree() <= c.n - 2
    return result
