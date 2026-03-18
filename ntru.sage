import hashlib  
import constants as c

shake_256 = hashlib.shake_256()
load('kem.sage')

def test_encryption(seed):
    m = hashlib.shake_256()
    m.update(bytes.fromhex(seed))
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
    (ss_encap, cipher) = Encapsulate(public_key)
    ss_decap = Decapsulate(private_key, cipher)
    #p_key = 


    #print(type(ss_decap), type(ss_encap))
    print(to_hex_str(ss_decap), "private key")
    print(to_hex_str(ss_encap), "private key")
    print(ss_encap == ss_decap)
    print(ss_encap, ss_decap)

def byte_to_byte(b):
    value = 0
    for (i, v) in enumerate(b):
        value += v << i
    return value

def to_hex_str(a):
    return ''.join([hex(b)[2:] for b in a])
