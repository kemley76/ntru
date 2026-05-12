#include "kem.h"
#include "bitstrings.h"
#include "constants.h"
#include "dpke.h"
#include "encodings.h"
#include "sampling.h"
#include "tests/arithmetic.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generates a public and a private key for use in the key exchange
KEM_Key_Pair_t Key_Pair(bitstring_t seed) {
    assert(seed.length == SAMPLE_KEY_BITS + PRF_KEY_BITS);
    bitstring_pair_t pair = split_at(seed, SAMPLE_KEY_BITS);

    bitstring_t fg_bits = pair.front;
    bitstring_t prf_key = pair.back;

    /*for (int i = 0; i < PRF_KEY_BITS / 8; i++) {
        printf("%02X \n", prf_key.data[i]);
    }*/

    uint8_t *packed_private_key = malloc(KEM_PRIVATE_KEY_BYTES);
    uint8_t *packed_public_key = malloc(KEM_PUBLIC_KEY_BYTES);
    DPKE_Key_Pair(fg_bits, packed_private_key, packed_public_key);

    memcpy(packed_private_key + DPKE_PRIVATE_KEY_BYTES, prf_key.data,
           PRF_KEY_BITS / 8);

    // idk why this one didn't work...
    // bits_to_bytes(prf_key, packed_private_key + DPKE_PRIVATE_KEY_BYTES);

    return (KEM_Key_Pair_t){.private_key = packed_private_key,
                            .public_key = packed_public_key};
}

// Generates a shared secret key and a ciphertext. The ciphertext can be
// used with the private key to recover the same shared secret key
KEM_Encapsualtion_t Encapsulate(uint8_t *packed_public_key, bitstring_t coins) {
    if (!coins.length) {
        coins = new_bitstring(SAMPLE_PLAINTEXT_BITS);
        // TODO: fill bits of coins randomly!
    }

    poly r = {0}, m = {0};
    Sample_rm(coins, &r, &m);

    uint8_t *packed_rm = malloc(PACKED_S3_BYTES * 2);
    pack_S3(&r, packed_rm);
    pack_S3(&m, packed_rm + PACKED_S3_BYTES);

    bitstring_t bitStringOut =
        bytes_to_bits(packed_rm, 8 * DPKE_PLAINTEXT_BYTES);

    for (int i = 0; i < DPKE_PLAINTEXT_BYTES; i++) {
        bitStringOut.data[i] = flip_byte(bitStringOut.data[i]);
    }
    uint8_t *shared_key = hash(bitStringOut);
    free(bitStringOut.data);

    uint8_t *packed_ciphertext = malloc(PACKED_RQ0_BYTES);
    DPKE_Encrypt(packed_public_key, packed_rm, packed_ciphertext);

    free(packed_rm);

    return (KEM_Encapsualtion_t){.shared_key = shared_key,
                                 .ciphertext = packed_ciphertext};
}

// Recovers the shared secret key from the ciphertext using the known private
// key There is a very small chance of failure, in which Decapsulate will return
// a random key
uint8_t *Decapsulate(uint8_t *packed_private_key, uint8_t *packed_ciphertext) {
    uint8_t *packed_dpke_private_key = packed_private_key;
    uint8_t *prf_key = packed_private_key + DPKE_PRIVATE_KEY_BYTES;

    uint8_t *packed_rm = malloc(PACKED_S3_BYTES * 2);
    DPKE_Decrypt(packed_dpke_private_key, packed_ciphertext, packed_rm);

    bitstring_t real_bits = bytes_to_bits(packed_rm, 8 * DPKE_PLAINTEXT_BYTES);
    free(packed_rm);

    for (int i = 0; i < real_bits.length / 8; i++) {
        real_bits.data[i] = flip_byte(real_bits.data[i]);
    }

    uint8_t *shared_key = hash(real_bits);
    free(real_bits.data);

    bitstring_t prf = bytes_to_bits(prf_key, PRF_KEY_BITS);
    bitstring_t cipher =
        bytes_to_bits(packed_ciphertext, 8 * KEM_CIPHERTEXT_BYTES);

    bitstring_t fake_bits = join(prf, cipher);
    for (int i = 0; i < fake_bits.length / 8; i++) {
        fake_bits.data[i] = flip_byte(fake_bits.data[i]);
    }
    uint8_t *random_key = hash(fake_bits);

    free(prf.data);
    free(cipher.data);
    free(fake_bits.data);

    int fail = 0; // TODO TODO TODO handle get failure info from DPKE_Decrypt
    if (fail) {
        free(shared_key);
        return random_key;
    }

    free(random_key);
    return shared_key;
}
