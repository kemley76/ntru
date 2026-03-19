import constants as c
from random import randint
from math import ceil
import hashlib
sha3_256 = hashlib.sha3_256()

load('dpke.sage')
load('encodings.sage')
load('sampling.sage')

def Key_Pair(seed):
    assert len(seed) == c.sample_key_bits + c.prf_key_bits, "invalid seed length"
    fg_bits = seed[:c.sample_key_bits]
    prf_key = seed[c.sample_key_bits:]

    (packed_dpke_private_key, packed_public_key) = DPKE_Key_Pair(fg_bits)
    packed_private_key = packed_dpke_private_key + bits_to_bytes(prf_key)

    assert len(packed_private_key) == c.kem_private_key_bytes, "Private key is wrong length"
    assert len(packed_public_key) == c.kem_public_key_bytes, "Public key is wrong length"
    return (packed_private_key, packed_public_key)

def Encapsulate(packed_public_key):
    coins = [randint(0, 1) for _ in range(c.sample_plaintext_bits)]
    (r, m) = Sample_rm(coins)
    # print("r:", r)
    # print("")
    # print("m:", m)
    packed_rm = pack_S3(r) + pack_S3(m)
    # print("Encapsulate:", packed_rm)
    # print(bytes_to_bits(packed_rm, 8*c.dpke_plaintext_bytes))
    bitStringOut = bytes_to_bits(packed_rm, 8*c.dpke_plaintext_bytes)
    shared_key = hash(bitStringOut)
    packed_ciphertext = DPKE_Encrypt(packed_public_key, packed_rm)
    return (shared_key, packed_ciphertext, m.list())

def Decapsulate(packed_private_key, packed_ciphertext):
    assert len(packed_private_key) == c.kem_private_key_bytes, "Invalid packed private key length"
    assert len(packed_ciphertext) == c.kem_ciphertext_bytes, "Invalid packed ciphertext length"
    packed_dpke_private_key = packed_private_key[:c.dpke_private_key_bytes]
    prf_key = packed_private_key[c.dpke_private_key_bytes:]
    # print(len(prf_key), ceil(c.prf_key_bits/8))
    assert len(packed_dpke_private_key) == c.dpke_private_key_bytes, "dpke private key is the wrong length"
    assert len(prf_key) == ceil(c.prf_key_bits/8), "prf key is the wrong length" 

    # Idk why the algorithm specifies this?
    packed_f = packed_private_key[:c.packed_s3_bytes]
    packed_fp = packed_private_key[c.packed_s3_bytes:c.packed_s3_bytes * 2] 
    packed_hq = packed_private_key[c.packed_s3_bytes * 2:]
    #assert len(packed_hq) == c.packed_s3_bytes

    (packed_rm, fail, packed_m) = DPKE_Decrypt(packed_dpke_private_key, packed_ciphertext)
    # print("Decapsulate:", packed_rm)
    
    decapBits = bytes_to_bits(packed_rm,8*c.dpke_plaintext_bytes)
    # print(bytes_to_bits(packed_rm,8*c.dpke_plaintext_bytes))
    # if decapBits[8*c.packed_s3_bytes] == 1:
    #     decapBits[8*c.packed_s3_bytes] = 0
    # else:
    #     decapBits[8*c.packed_s3_bytes] = 1

    shared_key = hash(bytes_to_bits(packed_rm,8*c.dpke_plaintext_bytes))
    random_key = hash(bytes_to_bits(prf_key, c.prf_key_bits) +  bytes_to_bits(packed_ciphertext, 8*c.kem_ciphertext_bytes) )
    if fail:
        # print("failed lol")
        return (random_key, packed_m)
    else: 
        return (shared_key, packed_m)



def hash(B):
    m = hashlib.sha3_256()
    # print([byte_to_byte(b) for b in B])
    # i=0
    # while i*8 < len(B):
    #     # print(ZZ(B[i*8:i*8+8], 2),' ', ZZ(B[i*8:i*8+8], 2).to_bytes(1,byteorder='little'), '\n')
    #     val = byte_to_byte(B[i*8:i*8+8])
    #     m.update(val.to_bytes(1,byteorder='little'))
    #     i+=1
    # print(i*8, len(B))
    # m.update(bytes_to_bits(B))
    # m.update(0.to_bytes(1,byteorder='big'))
    m.update(bytes(B))
    return m.digest()

def byte_to_byte(b):
    value = 0
    for (i, v) in enumerate(b):
        value += v << i
    return value
