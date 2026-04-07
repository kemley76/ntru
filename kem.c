#include "kem.h"
#include "bitstrings.h"
#include "constants.h"
#include "dpke.h"
#include "encodings.h"
#include "sampling.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generates a public and a private key for use in the key exchange
KEM_Key_Pair_t Key_Pair(bitstring_t seed) {
    fprintf(stderr, "Will finish!!\n");
    exit(EXIT_FAILURE);

    assert(seed.length == SAMPLE_KEY_BITS + PRF_KEY_BITS);
    bitstring_pair_t pair = split_at(seed, SAMPLE_KEY_BITS);

    bitstring_t fg_bits = pair.front;
    bitstring_t prf_key = pair.back;

    DPKE_key_pair_t key_pair; // = DPKE_Key_Pair(fg_bits);

    uint8_t *packed_private_key = malloc(KEM_PRIVATE_KEY_BYTES);
    uint8_t *packed_public_key = malloc(KEM_PUBLIC_KEY_BYTES);

    // combine the DPKE private key and the prf_key
    memcpy(packed_private_key, key_pair.packed_private_key,
           DPKE_PRIVATE_KEY_BYTES);
    bits_to_bytes(prf_key,
                  key_pair.packed_private_key + DPKE_PRIVATE_KEY_BYTES);

    return (KEM_Key_Pair_t){.private_key = packed_private_key,
                            .public_key = packed_public_key};
}

// Generates a shared secret key and a ciphertext. The ciphertext can be
// used with the private key to recover the same shared secret key
KEM_Encapsualtion_t Encapsulate(uint8_t *packed_public_key, bitstring_t coins) {
    fprintf(stderr, "Will finish!!\n");
    exit(EXIT_FAILURE);

    if (!coins.length) {
        coins = new_bistring(SAMPLE_PLAINTEXT_BITS);
        // TODO: fill bits of coins randomly!
    }

    // Polynomial_pair pair = Sample_rm(coins);
    // Polynomial r = pair.first;
    // Polynomial m = pair.second;

    uint8_t *packed_rm = malloc(PACKED_S3_BYTES * 2);
    // pack_S3(r, packed_rm);
    // pack_S3(m, packed_rm + PACKED_S3_BYTES) ;

    bitstring_t bitStringOut =
        bytes_to_bits(packed_rm, 8 * DPKE_PLAINTEXT_BYTES);
    uint8_t *shared_key;        // = hash(bitStringOut);
    uint8_t *packed_ciphertext; // = DPKE_Encrypt(packed_public_key, packed_rm);
    return (KEM_Encapsualtion_t){.shared_key = shared_key,
                                 .ciphertext = packed_ciphertext};
}

// Recovers the shared secret key from the ciphertext using the known private
// key There is a very small chance of failure, in which Decapsulate will return
// a random key
void Decapsulate() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Hashes a bitstring using sha3_256
void hash() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}
