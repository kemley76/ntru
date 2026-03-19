import ctypes
import constants as c
load('kem.sage')
lib = ctypes.CDLL('./librng.so')

lib.randombytes_init.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_ulonglong]
lib.randombytes_init.restype = None

lib.randombytes.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.c_ulonglong]
lib.randombytes.restype = ctypes.c_int
def test_kat():
    hex_seed = "061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1"
    seed = bytes.fromhex(hex_seed)
    lib.randombytes_init(seed, None, 256)

    # fg_bits
    size = int(c.sample_key_bits // 8)
    buffer = (ctypes.c_ubyte * size)()
    result = lib.randombytes(buffer, ctypes.c_ulonglong(size))
    assert result == 0 # success!
    bits = [(byte >> i) & 1 for byte in bytes(buffer) for i in range(8)]

    # prf_key
    size = int(c.prf_key_bits // 8)
    buffer = (ctypes.c_ubyte * size)()
    result = lib.randombytes(buffer, ctypes.c_ulonglong(size))
    assert result == 0 # success!
    bits += [(byte >> i) & 1 for byte in bytes(buffer) for i in range(8)]

    print(len(bits))
    #print(result, bytes(buffer))
    (private_key, public_key) = Key_Pair(bits)
    # print(bytes_to_hex(private_key), "private_key\n\n\n") # matches!
    #print(bytes_to_hex(public_key), "public_key\n\n\n")

    # prf_key
    #size = int(c.sample_plaintext_bits // 8)
    #buffer = (ctypes.c_ubyte * size)()
    #result = lib.randombytes(buffer, ctypes.c_ulonglong(size))
    #assert result == 0 # success!
    #bits = [(byte >> i) & 1 for byte in bytes(buffer) for i in range(8)]
    #(shared_key, ciphertext) = Encapsulate(public_key, bits)
    #print(shared_key.hex(), "shared secret\n\n\n")
    #print(ciphertext.hex(), "shared secret\n\n\n")

def bytes_to_hex(b):
    res = ''
    for byte in b:
        byte.reverse()
        res += hex(ZZ(byte, 2))[2:]
    return res

def to_hex_str(a):
    return ''.join([hex(b)[2:] for b in a])
