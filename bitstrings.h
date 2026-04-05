#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *data; // bits packed into an array of bytes
    size_t length; // number of bits in the bitstring
} bitstring_t;

bitstring_t new_bistring(size_t n);

uint8_t get_nth_bit(bitstring_t bits, size_t n);
