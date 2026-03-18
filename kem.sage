import constants as c
from random import randint
from math import ceil
import hashlib
sha3_256 = hashlib.sha3_256()

load('dpke.sage')
load('encodings.sage')

def Key_Pair(seed):
    assert len(seed) == c.sample_key_bits + c.prf_key_bits
    fg_bits = seed[:c.sample_key_bits]
    prf_key = seed[c.prf_key_bits:]

    (packed_dpke_private_key, packed_public_key) = DPKE_Key_Pair(fg_bits)
    packed_private_key = packed_dpke_private_key + bits_to_bytes(prf_key)
    return (packed_private_key, packed_public_key)

def Encapsulate(packed_public_key):
    coins = [randint(0, 1) for _ in range(c.sample_plaintext_bits)]
    (r, m) = Sample_rm(coins)
    packed_rm = pack_S3(r) + pack_S3(m)
    shared_key = hash(packed_rm)
    packed_ciphertext = DPKE_Encrypt(packed_public_key, packed_rm)
    return (shared_key, packed_ciphertext)

def Decapsulate(packed_private_key, packed_ciphertext):
    packed_dpke_private_key = packed_private_key[:c.dpke_private_key_bytes]
    prf_key = packed_private_key[c.dpke_private_key_bytes:]
    assert len(prf_key) == ceil(c.prf_key_bits/8)

    # Idk why the algorithm specifies this?
    #packed_f = packed_private_key[:c.packed_s3_bytes]
    #packed_fp = packed_private_key[c.packed_s3_bytes:c.packed_s3_bytes * 2] 
    #packed_hq = packed_private_key[c.packed_s3_bytes * 2:]
    #assert len(packed_hq) == c.packed_s3_bytes

    (packed_rm, fail) = DPKE_Decrypt(packed_dpke_private_key, packed_ciphertext)
    shared_key = hash(packed_rm)
    random_key = hash(prf_key + packed_ciphertext)
    if fail:
        return random_key
    else: 
        return shared_key

def hash(B):
    m = hashlib.sha3_256()
    m.update(bytes(B))
    return m.digest()

