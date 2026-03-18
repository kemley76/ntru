import hashlib  
import constants as c

shake_256 = hashlib.shake_256()
load('kem.sage')

def test_encryption(seed):
    m = hashlib.shake_256()
    m.update(seed)
    expanded_seed = m.digest((c.sample_key_bits + c.prf_key_bits) // 8)
    full_seed = []
    for b in expanded_seed:
        binary = bin(b)[2:]
        binary += '0' * (8 - len(binary))
        for bit in binary:
            if bit == '0': 
                full_seed += [0]
            else:
                full_seed += [1]


    (private_key, public_key) = Key_Pair(full_seed)
    (shared_key, packed_ciphertext) = Encapsulate(public_key)
    p_key = [hex(byte_to_byte(b))[2:] for b in private_key]
    print(''.join(p_key), "private key")

def byte_to_byte(b):
    value = 0
    for (i, v) in enumerate(b):
        value += v << i
    return value
