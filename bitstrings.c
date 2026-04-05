#include "bitstrings.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// n is the number of bits
bitstring_t new_bistring(size_t n) {
    // the + 1 is just in case n is not evenly divisible by 8
    return (bitstring_t){.data = malloc(n / 8 + 1), .length = n};
}

uint8_t get_nth_bit(bitstring_t bits, size_t n) {
    return (bits.data[n / 8] >> n % 8) & 0x1;
}

// splits a bitstring of n + m bits in one n of bits and one of m bits
// it is assumed that the split occurs on a byte boundary (n % 8 == 0)
bitstring_pair_t split_at(bitstring_t original, size_t n) {
    // we can point to the same data in the original because it splits on a byte
    // boundary. This assumes that the original won't get freed until we're done
    // with these
    bitstring_t front_half = (bitstring_t){.data = original.data, .length = n};
    bitstring_t back_half = new_bistring(original.length - n);
    back_half.data = original.data + (n / 8);

    return (bitstring_pair_t){front_half, back_half};
}
