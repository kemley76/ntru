import constants as c
load('constants.sage')
load('arithmetic.sage')

def Sample_fg(fg_bits):
	assert len(fg_bits) == c.sample_iid_bits * 2
	f_bits = fg_bits[:c.sample_iid_bits]
	g_bits = fg_bits[c.sample_iid_bits:]
	f = Ternary_Plus(f_bits)
	g_0 = Ternary_Plus(g_bits)
	g = phi_1 * g_0
	return (f, g)
	

def Sample_rm(rm_bits):
	assert len(rm_bits) == c.sample_iid_bits * 2
	r_bits = rm_bits[:c.sample_iid_bits]
	m_bits = rm_bits[c.sample_iid_bits:]
	r = Ternary(r_bits)
	m = Ternary(m_bits)
	return (r, m)

def Ternary(b):
	print(b)
	b.reverse()
	assert len(b) == c.sample_iid_bits 
	assert c.sample_iid_bits == 8 * (c.n - 1)
	v = 0
	i = 0
	while i < c.n - 1:
		v += sum([2^j * b[8 * i + j] for j in range(8)]) * x^i
		i += 1
	return S3_bar(v)

def Ternary_Plus(b):
	assert len(b) == c.sample_iid_bits, "input is not correct length"
	v = Ternary(b)
	coeffs = v.list()
	coeffs = [0] * (c.n - len(coeffs)) + coeffs
	assert len(coeffs) == c.n
	t = correlation(coeffs)
	print(t, c.n)
	assert -c.n < t and t < c.n, "correlation is outside expected range"
	s = 1
	if t < 0:
		s = -1
	i = 0
	while i < c.n - 1:
		coeffs[i] = s * coeffs[i] 
		i += 2
	result = sum([a * x^i for (i, a) in enumerate(coeffs)])

	assert correlation(result.list()) >= 0, "does not satisfy non-negative correlation condition" 
	return S3_bar(result)

def Fixed_Type(b):
	raise NotImplementedError("TODO!")

def correlation(coeffs): # Used to check the non-negative correlation property
    return sum([coeffs[i] * coeffs[i + 1] for i in range(0, len(coeffs) - 1)])
