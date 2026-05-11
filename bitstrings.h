#ifndef BITSTRINGS_H
#define BITSTRINGS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *data; // bits packed into an array of bytes
    size_t length; // number of bits in the bitstring
} bitstring_t;

typedef struct {
    bitstring_t front;
    bitstring_t back;
} bitstring_pair_t;

bitstring_t new_bitstring(size_t n);

void set_nth_bit(bitstring_t bits, size_t n, int val);

uint8_t get_nth_bit(bitstring_t bits, size_t n);

bitstring_pair_t split_at(bitstring_t original, size_t n);

// Combines two bistrings together
bitstring_t join(bitstring_t a, bitstring_t b);

#endif
