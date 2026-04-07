#include "sampling.h"
#include "arithmetic.h"
#include "constants.h"
#include "encodings.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial and another function to create a ternary polynomial with
// non-negative correlation
poly_pair Sample_fg(bitstring_t fg_bits) {
    bitstring_pair_t pair = split_at(fg_bits, SAMPLE_IID_BITS);
    bitstring_t f_bits = pair.front;
    bitstring_t g_bits = pair.back;

    // fix when polynomial type is added
    poly *f = Ternary_Plus(f_bits);
    poly *g_0 = Ternary_Plus(g_bits);

    poly *g;
    // poly *g = mod_mul(PHI_1, g_0);
    return (poly_pair){.first = f, .second = g};
}

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial from that bitstream
poly_pair Sample_rm(bitstring_t rm_bits) {
    assert(rm_bits.length == SAMPLE_PLAINTEXT_BITS);
    bitstring_pair_t pair = split_at(rm_bits, SAMPLE_IID_BITS);
    bitstring_t r_bits = pair.front;
    bitstring_t m_bits = pair.back;

    // fix when polynomial type is added
    poly *r = Ternary(r_bits);
    poly *m = Ternary(m_bits);

    return (poly_pair){.first = r, .second = m};
}

// takes in some bitstream, converts it into integers at 8 bits per int
// the passes the list of integers to S3_bar to reduce the result down to the S3
// ring
poly *Ternary(bitstring_t b) {
    printf("length %ld\n", b.length);
    assert(b.length == SAMPLE_IID_BITS);

    poly *v = calloc(1, sizeof(poly)); // create polynomial v = 0

    uint8_t *bytes = malloc(SAMPLE_IID_BITS / 8);
    bits_to_bytes(b, bytes);

    // Original algorithm: This seems like a complicated way of just copying the
    // individual bits into the coefficients??
    int i = 0;
    while (i < N) {
        v->coeffs[i] = bytes[i]; // Is this really how it would work?
        i++;
    }

    return S3_bar(v);
}

// takes in some bitstream, sums over the product of consecutive bits, check to
// see if this sum is less than one and then constructs either the negation or
// standard version of that polynomial in the S3 ring.
poly *Ternary_Plus(bitstring_t b) {
    assert(b.length == SAMPLE_IID_BITS);

    poly *v = Ternary(b);
    int t = 0;
    for (int i = 0; i < N - 2; i++) {
        t += v->coeffs[i] + 1 * v->coeffs[i + 1];
    }
    int s = t < 0 ? -1 : 1;
    int i = 0;
    while (i < N - 1) {
        v->coeffs[i] *= s;
        i += 2;
    }

    return S3_bar(v);
}

// put comment here
void correlation() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}
