import ctypes
import constants as c
from time import time
load('kem.sage')
lib = ctypes.CDLL('./librng.so')

lib.randombytes_init.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_ulonglong]
lib.randombytes_init.restype = None

lib.randombytes.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.c_ulonglong]
lib.randombytes.restype = ctypes.c_int

def test_all():
	f = open('PQCkemKAT_1450.rsp', 'r')
	f.readline()
	f.readline()
	pass_count = 0
	key_times = []
	enc_times = []
	dec_times = []
	for i in range(100):
		count = f.readline().split()[-1]
		seed = f.readline().split()[-1]
		known_pk = f.readline().split()[-1]
		known_sk = f.readline().split()[-1]
		known_ct = f.readline().split()[-1]
		known_ss = f.readline().split()[-1]
		f.readline()
		(pk, sk, ct, ss, ss2, key_time, enc_time, dec_time) = test_kat(seed)
		key_times.append(key_time)
		enc_times.append(enc_time)
		dec_times.append(dec_time)

		passes = [known_pk == pk, known_sk == sk, known_ct == ct, known_ss == ss, known_ss == ss2]
		print("Test", count)
		print("PK", passes[0])
		print("SK", passes[1])
		print("CT", passes[2])
		print("SS", passes[3])
		print("SS2", passes[4])
		print()
		if all(passes):
			pass_count += 1

	f.close()
	print(f"Total passes: {pass_count}/100")
	print("Average key gen time:", sum(key_times)/len(key_times))
	print("Average encapsulate time:", sum(enc_times)/len(enc_times))
	print("Average decapsulate time:", sum(dec_times)/len(dec_times))

def test_kat(hex_seed):
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


    key_start_time = time()
    (private_key, public_key) = Key_Pair(bits)
    key_time = time() - key_start_time

    # prf_key
    size = int(c.sample_plaintext_bits // 8)
    buffer = (ctypes.c_ubyte * size)()
    result = lib.randombytes(buffer, ctypes.c_ulonglong(size))
    assert result == 0 # success!
    bits = [(byte >> i) & 1 for byte in bytes(buffer) for i in range(8)]
    enc_start_time = time()
    (shared_key, ciphertext, _) = Encapsulate(public_key, coins=bits)
    enc_time = time() - enc_start_time

    dec_start_time = time()
    (shared_key2, _) = Decapsulate(private_key, ciphertext)
    dec_time = time() - dec_start_time
    return (bytes_to_hex(public_key), bytes_to_hex(private_key), bytes_to_hex(ciphertext),  
            shared_key.hex().upper(), shared_key2.hex().upper(), key_time, enc_time, dec_time)

def bytes_to_hex(b):
    res = ''
    for byte in b:
        hash_byte = copy.deepcopy(byte)
        hash_byte.reverse()
        res += format(ZZ(hash_byte, 2), '02X')
    return res

def to_hex_str(a):
    return ''.join([format(b, '02X') for b in a])
