import constants as c
from random import randint
from math import ceil
import hashlib

load('dpke.sage')
load('encodings.sage')
load('sampling.sage')

# Generates a public and a private key for use in the key exchange
def Key_Pair(seed):
    assert len(seed) == c.sample_key_bits + c.prf_key_bits, "invalid seed length"
    fg_bits = seed[:c.sample_key_bits]
    prf_key = seed[c.sample_key_bits:]

    (packed_dpke_private_key, packed_public_key) = DPKE_Key_Pair(fg_bits)
    packed_private_key = packed_dpke_private_key + bits_to_bytes(prf_key)

    assert len(packed_private_key) == c.kem_private_key_bytes, "Private key is wrong length"
    assert len(packed_public_key) == c.kem_public_key_bytes, "Public key is wrong length"
    return (packed_private_key, packed_public_key)

# Generates a shared secret key and a ciphertext. The ciphertext can be 
# used with the private key to recover the same shared secret key
def Encapsulate(packed_public_key, coins=None):
    if coins == None:
        coins = [randint(0, 1) for _ in range(c.sample_plaintext_bits)]
    (r, m) = Sample_rm(coins)
    packed_rm = pack_S3(r) + pack_S3(m)
    bitStringOut = bytes_to_bits(packed_rm, 8*c.dpke_plaintext_bytes)
    shared_key = hash(bitStringOut)
    packed_ciphertext = DPKE_Encrypt(packed_public_key, packed_rm)
    return (shared_key, packed_ciphertext, m.list())

# Recovers the shared secret key from the ciphertext using the known private key
# There is a very small chance of failure, in which Decapsulate will return a random key 
def Decapsulate(packed_private_key, packed_ciphertext):
    assert len(packed_private_key) == c.kem_private_key_bytes, "Invalid packed private key length"
    assert len(packed_ciphertext) == c.kem_ciphertext_bytes, "Invalid packed ciphertext length"
    packed_dpke_private_key = packed_private_key[:c.dpke_private_key_bytes]
    prf_key = packed_private_key[c.dpke_private_key_bytes:]
    assert len(packed_dpke_private_key) == c.dpke_private_key_bytes, "dpke private key is the wrong length"
    assert len(prf_key) == ceil(c.prf_key_bits/8), "prf key is the wrong length" 

    (packed_rm, fail, packed_m) = DPKE_Decrypt(packed_dpke_private_key, packed_ciphertext)
    shared_key = hash(bytes_to_bits(packed_rm,8*c.dpke_plaintext_bytes))
    random_key = hash(bytes_to_bits(prf_key, c.prf_key_bits) +  bytes_to_bits(packed_ciphertext, 8*c.kem_ciphertext_bytes) )
    if fail:
        return (random_key, packed_m)
    else: 
        return (shared_key, packed_m)

# Hashes a bitstring using sha3_256
def hash(B):
    m = hashlib.sha3_256()
    i=0
    while i*8 < len(B):
        # converts a chunk of 8 bits into a byte and adds it to the hash function buffer
        m.update(ZZ(B[i*8:i*8+8], 2).to_bytes(1,byteorder='little'))
        i+=1
    return m.digest()
