#include "bitstrings.h"
#include <stdint.h>
#include <stdlib.h>

// n is the number of bits
bitstring_t new_bistring(size_t n) {
    // the + 1 is just in case n is not evenly divisible by 8
    return (bitstring_t){.data = malloc(n / 8 + 1), .length = n};
}

uint8_t get_nth_bit(bitstring_t bits, size_t n) {
    return (bits.data[n / 8] >> n % 8) & 0x1;
}
