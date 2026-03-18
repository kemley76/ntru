import constants as c
load('sampling.sage')
load('arithmetic.sage')
load('encodings.sage')

# input: coins (bit string of length sample_key_bits)
# output: packed_private_key (byte array of length dpke_private_key_bytes)
#           packed_public_key (byte array of length dpke_public_key_bytes)
def DPKE_Key_Pair(coins):
    assert len(coins) == c.sample_key_bits
    (f,g) = Sample_fg(coins)
    f_p = S3_inverse(f)
    (h, h_q) = DPKE_Public_Key(f, g)
    packed_private_key = pack_S3(f) + pack_S3(f_p) + pack_Sq(h_q)
    packed_public_key = pack_Rq0(h)
    return (packed_private_key, packed_public_key)

# input: f (polynomial in lattice{f})
#       g (polynomial in lattice{g})
# output: h (polynomial that satisfies Rq(h · f) = 3 · g)
#           h_q (polynomial that satisfies Sq(h · hq) = 1)
def DPKE_Public_Key(f, g):
    G = 3 * g
    v_0 = Sq(G * f)
    v_1 = Sq_inverse(v_0) 
    h = Rq(v_1 * G * G)
    h_q = Rq(v_1 * f * f) 
    return (h, h_q)

# input: packed_public_key (byte array of length dpke_public_key_bytes)
#           packed_rm (byte array of length dpke_plaintext_bytes)
# output: packed_ciphertext (byte array of length dpke_ciphertext_bytes)
def DPKE_Encrypt(packed_public_key, packed_rm):
    assert len(packed_public_key) == c.dpke_public_key_bytes, "packed public key is wrong size"
    print(len(packed_rm), c.dpke_plaintext_bytes)
    assert len(packed_rm) == c.dpke_plaintext_bytes, "packed_rm is wrong size"

    packed_r = packed_rm[:c.packed_s3_bytes]
    packed_m = packed_rm[c.packed_s3_bytes:]
    assert len(packed_m) == c.packed_s3_bytes

    r = S3(unpack_S3(packed_r))
    m_0 = unpack_S3(packed_m)
    m_1 = Lift(m_0)
    h = unpack_Rq0(packed_public_key)
    cipher = Rq(r * h + m_1)
    packed_ciphertext = pack_Rq0(cipher)
    return packed_ciphertext

# input: packed_private_key (byte array of length dpke_private_key_bytes)
#           packed_ciphertext (byte array of length dpke_ciphertext_bytes)
# output: packed_rm (byte array of length dpke_plaintext_bytes)
#           fail (bit)
def DPKE_Decrypt(packed_private_key, packed_ciphertext):
    packed_f = packed_private_key[:c.packed_s3_bytes]
    packed_fp = packed_private_key[c.packed_s3_bytes:c.packed_s3_bytes * 2] 
    packed_hq = packed_private_key[c.packed_s3_bytes * 2:]
    assert len(packed_hq) == c.packed_s3_bytes
    cipher = unpack_Rq0(packed_ciphertext)
    f = S3(unpack_S3(packed_f))
    f_p = unpack_S3(packed_fp)
    h_q = unpack_Sq(packed_hq)
    v_1 = Rq(cipher * f)
    m_0 = S3(v_1 * f_p)
    m_1 = Lift(m_0)
    r = Sq((c - m_1) * h_q)
    packed_rm = pack_S3(r) + pack_S3(m_0)
    # TODO: Figure out how to check if r is in L_r and m_0 is in L_m
    return (packed_rm, 0)
