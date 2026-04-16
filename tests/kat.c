#include "kat.h"
#include "../rng.h"
#include "../utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parses the KAT file and checks and times our implementation against the tests
// contained in it
int test_all_kat() {
    FILE *fptr = fopen("PQCkemKAT_1450.rsp", "r");
    fscanf(fptr, "%*[^\n]\n"); // skips to the next line

    char seed[150];
    char pk[2300];
    char sk[3000];
    char ct[2300];
    char ss[100];
    int passed = 0;
    for (int i = 0; i < 1 /*00*/; i++) {
        fscanf(fptr, "%*[^\n]\n"); // skip count = X

        fscanf(fptr, "seed = %s\n", seed);
        fscanf(fptr, "pk = %s\n", pk);
        fscanf(fptr, "sk = %s\n", sk);
        fscanf(fptr, "ct = %s\n", ct);
        fscanf(fptr, "ss = %s\n", ss);
        printf("\n\ncount = %d\nseed: %s\nss: %s\n", i, seed, ss);

        uint8_t *seed_bytes = malloc(SAMPLE_KEY_BITS / 8);
        hex_to_bytes(seed, SAMPLE_KEY_BITS / 8, seed_bytes);
        passed += test_kat(seed_bytes, pk, sk, ct, ss);
    }
    printf("Total KATs passed: %d/100\n", passed);

    return 1;
}

int test_kat(uint8_t *seed, char *pk, char *sk, char *ct, char *ss) {
    randombytes_init(seed, NULL, 256);

    // fg_bits
    int size = SAMPLE_KEY_BITS / 8;
    bitstring_t bits = new_bistring(SAMPLE_KEY_BITS + PRF_KEY_BITS);
    int result = randombytes(bits.data, SAMPLE_KEY_BITS / 8);
    assert(result == 0);

    // prf_key
    result = randombytes(bits.data + SAMPLE_KEY_BITS / 8, PRF_KEY_BITS / 8);
    assert(result == 0);

    KEM_Key_Pair_t keypair = Key_Pair(bits);

    bitstring_t coins = new_bistring(SAMPLE_PLAINTEXT_BITS);
    result = randombytes(coins.data, SAMPLE_PLAINTEXT_BITS / 8);
    assert(result == 0);

    KEM_Encapsualtion_t capsule = Encapsulate(keypair.public_key, coins);
    uint8_t *shared_key = Decapsulate(keypair.private_key, capsule.ciphertext);

    char *actual_pk = malloc(KEM_PUBLIC_KEY_BYTES * 2 + 1);
    char *actual_sk = malloc(KEM_PRIVATE_KEY_BYTES * 2 + 1);
    char *actual_ct = malloc(KEM_CIPHERTEXT_BYTES * 2 + 1);
    char *actual_ss1 = malloc(KEM_SHARED_KEY_BITS / 8 * 2 + 1);
    char *actual_ss2 = malloc(KEM_SHARED_KEY_BITS / 8 * 2 + 1);
    bytes_to_hex(keypair.public_key, KEM_PUBLIC_KEY_BYTES, actual_pk);
    bytes_to_hex(keypair.private_key, KEM_PRIVATE_KEY_BYTES, actual_sk);
    bytes_to_hex(capsule.ciphertext, KEM_CIPHERTEXT_BYTES, actual_ct);
    bytes_to_hex(capsule.shared_key, KEM_SHARED_KEY_BITS / 8, actual_ss1);
    bytes_to_hex(shared_key, KEM_SHARED_KEY_BITS / 8, actual_ss2);

    if (strncmp(pk, actual_pk, KEM_PUBLIC_KEY_BYTES * 2)) {
        printf("test_kat: public key does not match expected\n%s\n\n%s\n", pk,
               actual_pk);
        return 0;
    }

    if (strncmp(sk, actual_sk, KEM_PRIVATE_KEY_BYTES * 2)) {
        printf("test_kat: private key does not match expected\n");
        printf("private key wrong: \n%s\n\n%s/\n", sk, actual_sk);
        return 0;
    }

    if (strncmp(ct, actual_ct, KEM_CIPHERTEXT_BYTES * 2)) {
        printf("test_kat: ciphertext does not match expected\n");
        printf("ciphertext: \n%s\n\n%s/\n", ct, actual_ct);
        return 0;
    }

    if (strncmp(ss, actual_ss1, KEM_SHARED_KEY_BITS / 8 * 2)) {
        printf("test_kat: shared secret (1) does not match expected\n");
        printf("shared secret: \n%s\n\n%s/\n", ss, actual_ss1);
        return 0;
    }

    if (strncmp(ss, actual_ss2, KEM_SHARED_KEY_BITS / 8 * 2)) {
        printf("test_kat: shared secret (2) does not match expected\n");
        return 0;
    }

    printf("test_kat: passed");
    return 1;
}
