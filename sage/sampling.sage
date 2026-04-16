import constants as c
load('encodings.sage')
load('arithmetic.sage')
# splits up the given bitstream of length twice sample iid bits into f and g bits 
# then passes each bitstream into a function to create a ternary polynomial and 
# another function to create a ternary polynomial with non-negative correlation
def Sample_fg(fg_bits):
	assert len(fg_bits) == c.sample_iid_bits * 2
	f_bits = fg_bits[:c.sample_iid_bits]
	g_bits = fg_bits[c.sample_iid_bits:]
	f = Ternary_Plus(f_bits)
	g_0 = Ternary_Plus(g_bits)
	g = PHI_1 * g_0
	#print("smapling G!!!", g)
	return (f, g)
# splits up the given bitstream of length twice sample iid bits into f and g bits 
# then passes each bitstream into a function to create a ternary polynomial from that bitstream
def Sample_rm(rm_bits):
    #print("RM BITS", bytes_to_hex(bits_to_bytes(rm_bits)))
    assert len(rm_bits) == c.sample_iid_bits * 2
    r_bits = rm_bits[:c.sample_iid_bits]
    m_bits = rm_bits[c.sample_iid_bits:]
    r = Ternary(r_bits)
    m = Ternary(m_bits)
    #print("output", bytes_to_hex(pack_S3(r)), bytes_to_hex(pack_S3(m)))

    return (r, m)
# takes in some bitstream, converts it into integers at 8 bits per int
# the passes the list of integers to S3_bar to reduce the result down to the S3 ring
def Ternary(b):
	assert len(b) == c.sample_iid_bits 
	assert c.sample_iid_bits == 8 * (c.n - 1)
	v = 0
	i = 0
	while i < c.n - 1:
		v += sum([2^j * b[8 * i + j] for j in range(8)]) * x^i
		#print("coeff", i, hex(sum([2^j * b[8 * i + j] for j in range(8)])))
		i += 1
	return S3_bar(v)
# takes in some bitstream, sums over the product of consecutive bits, check to see if this sum 
# is less than one and then constructs either the negation or standard version of that polynomial
# in the S3 ring.
def Ternary_Plus(b):
    assert len(b) == c.sample_iid_bits, "input is not correct length"
    v = Ternary(b)
    coeffs = v.list()
    coeffs = coeffs + [0] * (c.n - len(coeffs))
    assert len(coeffs) == c.n
    t = correlation(coeffs)
    assert -c.n < t and t < c.n, "correlation is outside expected range"
    s = 1
    if t < 0:
        s = -1
    i = 0
    while i < c.n - 1:
        coeffs[i] = s * coeffs[i] 
        #print("coeff:", i, coeffs[i]);
        i += 2
    result = sum([a * x^i for (i, a) in enumerate(coeffs)])

    assert correlation(result.list()) >= 0, "does not satisfy non-negative correlation condition" 
    return S3_bar(result)

def Fixed_Type(b):
	raise NotImplementedError("TODO!")

def correlation(coeffs): # Used to check the non-negative correlation property
    return sum([coeffs[i] * coeffs[i + 1] for i in range(0, len(coeffs) - 1)])

def bytes_to_hex(b):
    res = ''
    for byte in b:
        hash_byte = copy.deepcopy(byte)
        hash_byte.reverse()
        res += format(ZZ(hash_byte, 2), '02X')
    return res
