#ifndef KEM_H
#define KEM_H

#include "constants.h"
#include "dpke.h"
#include "encodings.h"
#include "sampling.h"
#include <stdint.h>
// need to include random, hashlib, and ceil from math but in C

typedef struct {
    uint8_t *private_key;
    uint8_t *public_key;
} KEM_Key_Pair_t;

// Generates a public and a private key for use in the key exchange
KEM_Key_Pair_t Key_Pair(bitstring_t seed);

typedef struct {
    uint8_t *shared_key;
    uint8_t *ciphertext;
} KEM_Encapsualtion_t;

// Generates a shared secret key and a ciphertext. The ciphertext can be
// used with the private key to recover the same shared secret key
KEM_Encapsualtion_t Encapsulate(uint8_t *packed_public_key, bitstring_t coins);

// Recovers the shared secret key from the ciphertext using the known private
// key There is a very small chance of failure, in which Decapsulate will return
// a random key
uint8_t *Decapsulate(uint8_t *packed_private_key, uint8_t *packed_ciphertext);

// Hashes a bitstring using sha3_256
uint8_t *hash(bitstring_t bits);

#endif
