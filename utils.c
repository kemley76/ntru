#include "bitstrings.h"
#include <openssl/evp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Need to have one byte extra for null terminator
void bytes_to_hex(uint8_t *bytes, int n_bytes, char *hex) {
    for (int i = n_bytes - 1; i >= 0; i--) {
        sprintf(hex + 2 * (n_bytes - 1 - i), "%02hhX", bytes[n_bytes - 1 - i]);
    }
}

void hex_to_bytes(char *hex, int n_bytes, uint8_t *bytes) {
    for (int i = 0; i < n_bytes; i++) {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}

// Hashes a bitstring using sha3_256
uint8_t *hash(bitstring_t bits) {
    size_t hash_len;
    size_t len = bits.length / 8;
    if (bits.length % 8 != 0)
        len++;

    uint8_t *out_hash = malloc(32);
    if (!EVP_Q_digest(NULL, "SHA3-256", NULL, bits.data, len, out_hash,
                      &hash_len)) {
        printf("error getting hash\n");
    }

    return out_hash;
}
