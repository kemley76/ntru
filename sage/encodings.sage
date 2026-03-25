from math import ceil 
from math import log2
import constants as c
import copy
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
        test_byte = copy.deepcopy(byte)
        test_byte.reverse()
        for bit in test_byte:
            bits_out.append(int(bit))
    bits_out += (length - len(bits_out)) * [0]
    return bits_out

# takes in some polynomial a, finds its representation in the Rq ring, and extracts all coefficients of post rq representation. 
# These coefficients are converted to binary and packed into a list for further operation
def pack_Rq0(a):
    assert a in Z, "input is not a polynomial"
    remainder = a % PHI_1
    assert all(coeff % c.q == 0 for coeff in remainder.list()), "a is not equiv to zero"
    
    v = Rq_bar(a)
    b = []
    coeffs = v.list()
    coeffs = coeffs + [0] * (c.n - len(coeffs)) # pad to have n coeficients
    for coeff in coeffs[:-1]:
        b += (coeff % c.q).digits(2, padto=logq)
    result = bits_to_bytes(b)
    assert len(result) == c.packed_rq0_bytes, "result is wrong length"
    return result

# takes in some list of bytes and converts these bytes into integer coefficients
# using some given length of bits / coefficient log2q
def unpack_Rq0(B):
    assert len(B) == c.packed_rq0_bytes, "input is wrong length"
    coeffs = []
    bits = bytes_to_bits(B, (c.n - 1) * logq)

    for i in range(0, (c.n - 1) * logq, logq):
        coeffs.append(ZZ(bits[i:i+logq], 2))
    
    last_coeff = (-sum(coeffs)) % c.q
    coeffs.append(last_coeff)

    a = Rq(coeffs)
    # for assertion:
    remainder = a % PHI_1
    assert all(coeff % c.q == 0 for coeff in remainder.list()), "a is not equiv to zero"

    return a

# takes in some polynomial a, evalutes a in the ring Sq, extracts its coefficients, and converts its post sq 
# coefficients to binary with a given length per number. 
def pack_Sq(a):
    assert a in Z, "input is not a polynomial"
    v = Sq_bar(a)
    b = []
    coeffs = v.list()
    coeffs = coeffs + [0] * (c.n - len(coeffs)) # pad to have n coeficients
    for coeff in coeffs[:-1]:
        b += (coeff % c.q).digits(2, padto=logq)
    result = bits_to_bytes(b)
    assert len(result) == c.packed_sq_bytes, "result is wrong length"
    return result

# takes in some list of bytes, forms a list of bits, breaks up the list of bits into sections of length
# log2(q) and then passes the result to Sq normative form. 
def unpack_Sq(B):
    assert len(B) == c.packed_sq_bytes, "input is wrong length"
    coeffs = []
    bits = bytes_to_bits(B, (c.n - 1) * logq)

    for i in range(0, (c.n - 1) * logq, logq):
        coeffs.append(ZZ(bits[i:i+logq], 2))
    return Sq(Z(coeffs))

# takes in some polynomial a, evalutes a in the ring S3, extracts its post transformation coefficients, 
# and converts its post s3  coefficients to binary with a given length per number. 
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

# takes in some list of bytes, forms a list of bits, breaks up the list of bits into sections of length
# 8 and then conducts a change of basis operation to ternary bytes of length 5. These bytes are used as
# coefficients of V and rerepresented in S3. 
def unpack_S3(B):
    i = 0
    bits = bytes_to_bits(B, 8*ceil((c.n - 1) / 5))
    v = []
    while i < ceil((c.n - 1) / 5):
        coeffs = []
        coeffs.append(ZZ(bits[i*8:i*8+8], 2).digits(3,padto=5))
        for coe in coeffs[0]:
            v.append(coe)
        i+= 1
    return S3(Z(v))

