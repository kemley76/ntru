from math import ceil 
from math import log2
import constants as c
load('arithmetic.sage')
logq = int(log2(c.q))

# converts some string of bits into an array of bit arrays. 
# each bit array is length eight composed of 0 and 1 integers.
# the order follows [b1 b2 b3 b4 b5 b6 b7 b8 b9] into 
# [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
def bits_to_bytes(bits_in):
    numBits = len(bits_in)
    numLoops = ceil(numBits / 8)
    bytes_out = []
    for loop in range(0, numLoops):
        curByte = [0, 0, 0, 0, 0, 0, 0, 0]
        for bit in range(0, 8):
            if (bit+(8*loop)) < numBits:
                curByte[bit] = int(bits_in[bit+8*loop])
        curByte.reverse()
        bytes_out.append(curByte)
    return bytes_out

# converts some array of bytes into an array of bits. 
# byte arrays are flipped then appended to an empty bit array
# the order follows [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]] 
# into [b1 b2 b3 b4 b5 b6 b7 b8 b9]
def bytes_to_bits(bytes_in, length):
    bits_out = []
    for byte in bytes_in:
        byte.reverse()
        for bit in byte:
            bits_out.append(int(bit))
    return bits_out

def pack_Rq0(a):
    assert a in Z, "input is not a polynomial"
    v = Rq_bar(a)
    b = []
    coeffs = v.list()
    coeffs = coeffs + [0] * (c.n - len(coeffs)) # pad to have n coeficients
    for coeff in coeffs[:-1]:
        b += (coeff % c.q).digits(2, padto=logq)
    result = bits_to_bytes(b)
    assert len(result) == c.packed_rq0_bytes, "result is wrong length"
    return result

def unpack_Rq0(B):
    assert len(B) == c.packed_rq0_bytes, "input is wrong length"
    coeffs = []
    bits = bytes_to_bits(B, (c.n - 1) * logq)

    for i in range(0, (c.n - 1) * logq, logq):
        coeffs.append(ZZ(bits[i:i+logq], 2))
    return Rq_bar(Z(coeffs))	

def pack_Sq(a):
    # assert a in Z, "input is not a polynomial"
    v = Sq_bar(a)
    b = []
    coeffs = v.list()
    coeffs = coeffs + [0] * (c.n - len(coeffs)) # pad to have n coeficients
    for coeff in coeffs[:-1]:
        b += (coeff % c.q).digits(2, padto=logq)
    result = bits_to_bytes(b)
    assert len(result) == c.packed_sq_bytes, "result is wrong length"
    return result
	
def unpack_Sq(B):
    assert len(B) == c.packed_sq_bytes, "input is wrong length"
    coeffs = []
    bits = bytes_to_bits(B, (c.n - 1) * logq)

    for i in range(0, (c.n - 1) * logq, logq):
        coeffs.append(ZZ(bits[i:i+logq], 2))
    return Sq_bar(Z(coeffs))

def pack_S3(a):
    v = S3_bar(a)
    b = []
    i = 0
    coeffs = v.list()
    coeffs = coeffs + [0] * (c.n - 1 - len(coeffs)) # pad to have n - 1 coefficients
    while i < ceil((c.n - 1) / 5):
        co = [x % 3 for x in coeffs[5 * i:5 * i + 5]]
        b += ZZ(co, 3).digits(2, padto=8)
        i += 1
    result = bits_to_bytes(b)
    assert len(result) == c.packed_s3_bytes, "pack s3 has wrong length result"
    return result

def unpack_S3(B):
    coeffs = []
    for byte in B:
        byte.reverse()
        coeffs += ZZ(byte, 2).digits(3, padto=5)
    return S3_bar(Z(coeffs))
