#include "encodings.h"
#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// converts some string of bits into an array of bit arrays.
// each bit array is length eight composed of 0 and 1 integers.
// the order follows [b1 b2 b3 b4 b5 b6 b7 b8 b9] into
// [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
void bits_to_bytes(bitstring_t bits, uint8_t *bytes) {
    int byte_count = bits.length / 8;
    if (bits.length % 8 != 0)
        byte_count++;
    for (int i = 0; i < byte_count; i++) {
        uint8_t r = bits.data[i]; // raw bits (lsb first)
                                  // flip the bits to have msb first
        bytes[i] = ((r & 0b1) << 7) | ((r & 0b10) << 5) | ((r & 0b100) << 3) |
                   ((r & 0b1000) << 1) | ((r & 0b10000) >> 1) |
                   ((r & 0b100000) >> 3) | ((r & 0b1000000) >> 5) |
                   ((r & 0b10000000) >> 7);
    }
}

// converts some array of bytes into an array of bits.
// byte arrays are flipped then appended to an empty bit array
// the order follows [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
// into [b1 b2 b3 b4 b5 b6 b7 b8 b9]
// n is the number of bits
bitstring_t bytes_to_bits(uint8_t *bytes, size_t n) {
    bitstring_t bits = new_bistring(n);
    int byte_count = n / 8;
    if (n % 8 != 0)
        byte_count++;

    for (int i = 0; i < byte_count; i++) {
        uint8_t r = bytes[i];
        bits.data[i] = ((r & 0b1) << 7) | ((r & 0b10) << 5) |
                       ((r & 0b100) << 3) | ((r & 0b1000) << 1) |
                       ((r & 0b10000) >> 1) | ((r & 0b100000) >> 3) |
                       ((r & 0b1000000) >> 5) | ((r & 0b10000000) >> 7);
    }

    return bits;
}

// takes in some polynomial a, finds its representation in the Rq ring, and
// extracts all coefficients of post rq representation. These coefficients are
// converted to binary and packed into a list for further operation
void pack_Rq0() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some list of bytes and converts these bytes into integer
// coefficients using some given length of bits / coefficient log2q
void unpack_Rq0() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some polynomial a, evalutes a in the ring Sq, extracts its
// coefficients, and converts its post sq coefficients to binary with a given
// length per number.
void pack_Sq() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length log2(q) and then passes the result to Sq normative
// form.
void unpack_Sq() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some polynomial a, evalutes a in the ring S3, extracts its post
// transformation coefficients, and converts its post s3  coefficients to binary
// with a given length per number.
uint8_t *pack_S3(poly *a) {
    poly *v = S3_bar(a);
    uint8_t *result = malloc(PACKED_S3_BYTES);

    for (int i = 0; i < (N - 1) / 5; i++) {
        int c = 0;
        int power = 1;
        for (int j = 0; j < 5; j++) {
            c += power * (v->coeffs[5 * i + j] % 3);
            power *= 3;
        }
        assert(c < (2 << 8));
        result[i] = c;
    }

    return result;
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length 8 and then conducts a change of basis operation to
// ternary bytes of length 5. These bytes are used as coefficients of V and
// rerepresented in S3.
poly *unpack_S3(uint8_t *B) {
    poly *v = calloc(1, sizeof(poly)); // create polynomial v = 0
    for (int i = 0; i < (N - 1) / 5; i++) {
        uint8_t byte = B[i];
        uint8_t c[5];
        for (int j = 0; j < 5; j++) {
            v->coeffs[5 * i + 1] = byte % 3;
            byte /= 3;
        }
    }

    return S3(v);
}
