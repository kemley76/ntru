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


    #print(private_key, type(ss_encap))

    # print(bytes_to_hex(private_key), "private key")
    # print(bytes_to_hex(public_key), "public key key")
    print(ss_encap == ss_decap)
    #print(ss_encap, ss_decap)

def byte_to_byte(b):
    value = 0
    for (i, v) in enumerate(b):
        value += v << i
    return value

def bytes_to_hex(b):
    return to_hex_str([byte_to_byte(byte) for byte in b])

def to_hex_str(a):
    return ''.join([hex(b)[2:] for b in a])
