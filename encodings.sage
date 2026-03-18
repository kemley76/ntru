<<<<<<< HEAD
# converts some string of bits into an array of bit arrays. 
# each bit array is length eight composed of 0 and 1 integers.
# the order follows [b1 b2 b3 b4 b5 b6 b7 b8 b9] into 
# [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
def bits_to_bytes(bits_in):
    numBits = len(bits_in)
    numLoops = (numBits // 8) + 1
    bytes_out = []
    for loop in range(0, numLoops):
        curByte = [0, 0, 0, 0, 0, 0, 0, 0]
        for bit in range(0, 8):
            if (bit+(8*loop)) < numBits:
                curByte[bit] = bits_in[bit+8*loop]
        curByte.reverse()
        bytes_out.append(curByte)
    return bytes_out

# converts some array of bytes into an array of bits. 
# byte arrays are flipped then appended to an empty bit array
# the order follows [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]] 
# into [b1 b2 b3 b4 b5 b6 b7 b8 b9]
def bytes_to_bits(bytes_in):
    bits_out = []
    for byte in bytes_in:
        byte.reverse()
        for bit in byte:
            bits_out.append(bit)
    return bits_out

# gets the coefficients of some input polynomial
# loops through to convert each input coefficient into a bit string
# normalizes the length of each bit string to log(q)
# the last coefficient v_n-1 isn't encoded
# once all normalized length bitstreams are assembled they're given
# to bits_to_bytes
# def pack_Rq0(polyA):
#     v = Rq(polyA)
#     # logq = 9
#     v = [141, 255, 177, 61, 24, 19]
#     n_1 = len(polyA) - 1
#     # n_1 = 5
#     bits = []
#     for loop in range(0, n_1):
#         simpleBit = [0] * (logq)
#         # note that this works without the mod but I'll need to update this
#         # in post to reference the right q
#         numBin = list(bin(v[loop] % q)[2:])
#         for dig in range(0,len(numBin)):
#             simpleBit[dig] = numBin[dig]
#         for dig in range(0,len(simpleBit)):
#             bits.append(simpleBit[dig])
#     return bits_to_bytes(bits)

# def unpack_Rq0(bytes_in):
#     return poly_coef

#     ## I need to write the KEM file.
    
# # this is pack rq0 but with Sq instead of Rq on the first
# # line after the function def
# def pack_Sq0(polyA):
#     v = Sq(polyA)
#     # logq = 9
#     v = [141, 255, 177, 61, 24, 19]
#     n_1 = len(polyA) - 1
#     # n_1 = 5
#     bits = []
#     for loop in range(0, n_1):
#         simpleBit = [0] * (logq)
#         # note that this works without the mod but I'll need to update this
#         # in post to reference the right q
#         numBin = list(bin(v[loop] % q)[2:])
#         for dig in range(0,len(numBin)):
#             simpleBit[dig] = numBin[dig]
#         for dig in range(0,len(simpleBit)):
#             bits.append(simpleBit[dig])
#     return bits_to_bytes(bits)

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
