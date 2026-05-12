#include "kat.h"
#include "../rng.h"
#include "../utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Timing variables
struct timespec start, end;
long long total_keygen_time = 0;
long long total_enc_time = 0;
long long total_dec_time = 0;

// Parses the KAT file and checks and times our implementation against the tests
// contained in it
int test_all_kat() {
    FILE *fptr = fopen("PQCkemKAT_1450.rsp", "r");
    fscanf(fptr, "%*[^\n]\n"); // skips to the next line

    int passed = 0;

    for (int i = 0; i < 100; i++) {
        char seed[SAMPLE_KEY_BITS / 4] = {0}; // Not exact sizes
        char pk[2300] = {0};
        char sk[3000] = {0};
        char ct[2300] = {0};
        char ss[100] = {0};

        fscanf(fptr, "%*[^\n]\n"); // skip count = X

        fscanf(fptr, "seed = %s\n", seed);
        fscanf(fptr, "pk = %s\n", pk);
        fscanf(fptr, "sk = %s\n", sk);
        fscanf(fptr, "ct = %s\n", ct);
        fscanf(fptr, "ss = %s\n", ss);

        uint8_t seed_bytes[SAMPLE_KEY_BITS / 8] = {0};
        hex_to_bytes(seed, SAMPLE_KEY_BITS / 8, seed_bytes);
        int current = test_kat(seed_bytes, pk, sk, ct, ss);
        if (current)
            printf("KAT %d passed\n", i);
        else
            printf("KAT %d failed\n", i);

        passed += current;
    }
    printf("Total KATs passed: %d/100\n", passed);
    printf("  Key Generation:\t%lld microseconds\n",
           total_keygen_time / 100 / 1000);
    printf("  Encapsulation:\t%lld microseconds\n",
           total_enc_time / 100 / 1000);
    printf("  Decapsulation:\t%lld microseconds\n",
           total_dec_time / 100 / 1000);

    return 1;
}

int test_kat(uint8_t *seed, char *pk, char *sk, char *ct, char *ss) {
    randombytes_init(seed, NULL, 256);

    // fg_bits
    int size = SAMPLE_KEY_BITS / 8;
    bitstring_t bits = new_bitstring(SAMPLE_KEY_BITS + PRF_KEY_BITS);
    int result = randombytes(bits.data, SAMPLE_KEY_BITS / 8);
    assert(result == 0);

    // prf_key
    result = randombytes(bits.data + SAMPLE_KEY_BITS / 8, PRF_KEY_BITS / 8);
    assert(result == 0);

    clock_gettime(CLOCK_MONOTONIC, &start);
    KEM_Key_Pair_t keypair = Key_Pair(bits);
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_keygen_time += (end.tv_sec - start.tv_sec) * 1000000000 +
                         (end.tv_nsec - start.tv_nsec);
    free(bits.data);

    bitstring_t coins = new_bitstring(SAMPLE_PLAINTEXT_BITS);
    result = randombytes(coins.data, SAMPLE_PLAINTEXT_BITS / 8);
    assert(result == 0);

    clock_gettime(CLOCK_MONOTONIC, &start);
    KEM_Encapsualtion_t capsule = Encapsulate(keypair.public_key, coins);
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_enc_time += (end.tv_sec - start.tv_sec) * 1000000000 +
                      (end.tv_nsec - start.tv_nsec);
    free(coins.data);

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint8_t *shared_key = Decapsulate(keypair.private_key, capsule.ciphertext);
    clock_gettime(CLOCK_MONOTONIC, &end);
    total_dec_time += (end.tv_sec - start.tv_sec) * 1000000000 +
                      (end.tv_nsec - start.tv_nsec);

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
    free(keypair.public_key);
    free(keypair.private_key);
    free(capsule.ciphertext);
    free(capsule.shared_key);
    free(shared_key);

    int ret = 1;
    if (strncmp(pk, actual_pk, KEM_PUBLIC_KEY_BYTES * 2)) {
        printf("test_kat: public key does not match expected\n");
        printf("public key wrong: \n%s\n%s\n", pk, actual_pk);
        ret = 0;
    }

    if (strncmp(sk, actual_sk, KEM_PRIVATE_KEY_BYTES * 2)) {
        printf("test_kat: private key does not match expected\n");
        printf("private key wrong: \n%s\n\n%s\n", sk, actual_sk);
        ret = 0;
    }

    if (strncmp(ct, actual_ct, KEM_CIPHERTEXT_BYTES * 2)) {
        printf("test_kat: ciphertext does not match expected\n");
        printf("ciphertext: \n%s\n\n%s\n", ct, actual_ct);
        ret = 0;
    }

    if (strncmp(ss, actual_ss1, KEM_SHARED_KEY_BITS / 8 * 2)) {
        printf("test_kat: shared secret (1) does not match expected\n");
        printf("shared secret: \n%s\n\n%s\n", ss, actual_ss1);
        ret = 0;
    }

    if (strncmp(ss, actual_ss2, KEM_SHARED_KEY_BITS / 8 * 2)) {
        printf("test_kat: shared secret (2) does not match expected\n");
        printf("shared secret: \n%s\n\n%s\n", ss, actual_ss2);
        ret = 0;
    }

    free(actual_pk);
    free(actual_sk);
    free(actual_ct);
    free(actual_ss1);
    free(actual_ss2);
    return ret;
}
