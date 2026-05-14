#include "bitstrings.h"
#include "fips202.h"
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
        if (hex[2 * i] == 0)
            bytes[i] = 0;
        else
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
    sha3_256(out_hash, bits.data, len);

    return out_hash;
}

// Standard ARM addresses for the DWT Cycle Counter
volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004;
volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC;
uint32_t start = 0;

void start_measure() {
    // Enable the trace unit and the cycle counter
    /*printf("starting measure!!\n");
    *DEMCR |= 0x01000000;
    *DWT_CONTROL |= 1;

    start = *DWT_CYCCNT;*/
}

void end_measure() {
    /*uint32_t end = *DWT_CYCCNT;
    uint32_t cycles = end - start;

    printf("Total clock cycles: %d\n", cycles);*/
}
