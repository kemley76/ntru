# input: coins (bit string of length sample_key_bits)
# output: packed_private_key (byte array of length dpke_private_key_bytes)
#           packed_public_key (byte array of length dpke_public_key_bytes)
def DPKE_Key_Pair(coins):
    # unimplemented
    raise NotImplementedError("This function has not been implemented yet.")

# input: f (polynomial in lattice{f})
#       g (polynomial in lattice{g})
# output: h (polynomial that satisfies Rq(h · f) = 3 · g)
#           h_q (polynomial that satisfies Sq(h · hq) = 1)
def DPKE_Public_Key(f, g):
    # unimplemented
    raise NotImplementedError("This function has not been implemented yet.")

# input: packed_public_key (byte array of length dpke_public_key_bytes)
#           packed_rm (byte array of length dpke_plaintext_bytes)
# output: packed_ciphertext (byte array of length dpke_ciphertext_bytes)
def DPKE_Encrypt(packed_public_key, packed_rm):
    # unimplemented
    raise NotImplementedError("This function has not been implemented yet.")

# input: packed_private_key (byte array of length dpke_private_key_bytes)
#           packed_ciphertext (byte array of length dpke_ciphertext_bytes)
# output: packed_rm (byte array of length dpke_plaintext_bytes)
#           fail (bit)
def DPKE_Decrypt(packed_private_key, packed_ciphertext):
    # unimplemented
    raise NotImplementedError("This function has not been implemented yet.")