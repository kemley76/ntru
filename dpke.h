#ifndef DPKE_H
#define DPKE_H

#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include "encodings.h"
#include "sampling.h"
#include <stdint.h>

typedef struct {
    uint8_t *packed_private_key;
    uint8_t *packed_public_key;
} DPKE_key_pair_t;

// input: coins (bit string of length sample_key_bits)
// output: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_public_key (byte array of length dpke_public_key_bytes)
DPKE_key_pair_t DPKE_Key_Pair(bitstring_t coins);

// input: f (polynomial in lattice{f})
//       g (polynomial in lattice{g})
// output: h (polynomial that satisfies Rq(h · f) = 3 · g)
//           h_q (polynomial that satisfies Sq(h · hq) = 1)
void DPKE_Public_Key(poly *f, poly *g, poly *h, poly *h_q);

// input: packed_public_key (byte array of length dpke_public_key_bytes)
//           packed_rm (byte array of length dpke_plaintext_bytes)
// output: packed_ciphertext (byte array of length dpke_ciphertext_bytes)
uint8_t *DPKE_Encrypt(uint8_t *packed_public_key, uint8_t *packed_rm);

// input: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_ciphertext (byte array of length dpke_ciphertext_bytes)
// output: packed_rm (byte array of length dpke_plaintext_bytes)
//           fail (bit)
uint8_t *DPKE_Decrypt(uint8_t *packed_private_key, uint8_t *packed_ciphertext);

#endif
