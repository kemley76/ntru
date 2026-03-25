#include "dpke.h"

// input: coins (bit string of length sample_key_bits)
// output: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_public_key (byte array of length dpke_public_key_bytes)
void DPKE_Key_Pair(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// input: f (polynomial in lattice{f})
//       g (polynomial in lattice{g})
// output: h (polynomial that satisfies Rq(h · f) = 3 · g)
//           h_q (polynomial that satisfies Sq(h · hq) = 1)
void DPKE_Public_Key(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// input: packed_public_key (byte array of length dpke_public_key_bytes)
//           packed_rm (byte array of length dpke_plaintext_bytes)
// output: packed_ciphertext (byte array of length dpke_ciphertext_bytes)
void DPKE_Encrypt(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// input: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_ciphertext (byte array of length dpke_ciphertext_bytes)
// output: packed_rm (byte array of length dpke_plaintext_bytes)
//           fail (bit)
void DPKE_Decrypt(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}