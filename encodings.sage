from math import log2
import constants as c
load('constants.sage')
load('arithmetic.sage')
load('encodings.sage')

logq = int(log2(c.q))

def pack_Rq0(a):
    assert a in P, "input is not a polynomial"
    assert (a % phi_1) % c.q == 0, "input is not in Rq0" # I'm not sure if this is the way to check 
    v = Rq(a)
    b = []
    i = 0
    coeffs = v.list()
    while i < c.n - 1:
        if i in coeffs:
            b += int_to_bits(coeffs[i], logq)
        else:
            b += int_to_bits(0, logq)
        i += 1
    result = bits_to_bytes(b)
    assert len(result) == c.packed_rq0_bytes
    return result

def unpack_Rq0(B):
    assert len(B) == c.packed_rq0_bytes, "input is wrong length"
    b = bytes_to_bits(B, (c.n - 1) * logq)
    v = 0
    i = 0
    while i < c.n - 1:
        print(i, logq)
        co = sum([2^j * b[i * logq + j] for j in range(logq)])
        v += v + co * x^i - co * x^(c.n - 1)
        i += 1
    return Rq(v)	

def pack_Sq(a):
    assert a in P, "input is not a polynomial"
    v = Rq(a)
    b = []
    i = 0
    coeffs = v.list()
    while i < c.n - 1:
        if i in coeffs:
            b += int_to_bits(coeffs[i], logq)
        else:
            b += int_to_bits(0, logq)
        i += 1
    result = bits_to_bytes(b)
    assert len(result) == c.packed_sq_bytes
    return result
	
def unpack_Sq(B):
    assert len(B) == c.packed_sq_bytes, "input is wrong length"
    b = bytes_to_bits(B, (c.n - 1) * logq)
    v = 0
    i = 0
    while i < c.n - 1:
        print(i, logq)
        co = sum([2^j * b[i * logq + j] for j in range(logq)])
        v += v + co * x^i - co * x^(c.n - 1)
        i += 1
    return Sq(v) # need to implement still...

def pack_S3(a):
    assert a in P, "input is not a polynomial"
    v = Rq(a)
    b = []
    i = 0
    coeffs = v.list()
    while i < c.n - 1:
        if i in coeffs:
            b += int_to_bits(coeffs[i], logq)
        else:
            b += int_to_bits(0, logq)
        i += 1
    result = bits_to_bytes(b)
    assert len(result) == c.packed_sq_bytes
    return result

def unpack_S3(a):
	raise NotImplementedError("TODO!")

def int_to_bits(n, width):
	return [(n >> i) & 1 for i in range(width)]
