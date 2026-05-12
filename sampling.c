#include "sampling.h"
#include "./tests/arithmetic.h"
#include "arithmetic.h"
#include "constants.h"
#include "encodings.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

poly PHI_1 = {.coeffs = {[0] = -1, [1] = 1}};

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial and another function to create a ternary polynomial with
// non-negative correlation
void Sample_fg(bitstring_t fg_bits, poly *out_f, poly *out_g) {
    bitstring_pair_t pair = split_at(fg_bits, SAMPLE_IID_BITS);
    bitstring_t f_bits = pair.front;
    bitstring_t g_bits = pair.back;

    // fix when polynomial type is added
    Ternary_Plus(f_bits, out_f);

    poly g_0 = {0};
    Ternary_Plus(g_bits, &g_0);

    // g = g_0 * PHI_1
    for (int i = 0; i < N; i++) {
        out_g->coeffs[i] = g_0.coeffs[(i + N - 1) % N] - g_0.coeffs[i];
    }
}

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial from that bitstream
void Sample_rm(bitstring_t rm_bits, poly *out_r, poly *out_m) {
    assert(rm_bits.length == SAMPLE_PLAINTEXT_BITS);
    bitstring_pair_t pair = split_at(rm_bits, SAMPLE_IID_BITS);
    bitstring_t r_bits = pair.front;
    bitstring_t m_bits = pair.back;

    // fix when polynomial type is added
    Ternary(r_bits, out_r);
    Ternary(m_bits, out_m);
}

// takes in some bitstream, converts it into integers at 8 bits per int
// the passes the list of integers to S3_bar to reduce the result down to the S3
// ring
void Ternary(bitstring_t b, poly *out) {
    assert(b.length == SAMPLE_IID_BITS);

    // poly *v = calloc(1, sizeof(poly)); // create polynomial v = 0

    uint8_t *bytes = calloc(1, SAMPLE_IID_BITS / 8);
    bits_to_bytes(b, bytes);

    // Original algorithm: This seems like a complicated way of just copying the
    // individual bits into the coefficients??
    int i = 0;
    while (i < N) {
        out->coeffs[i] =
            flip_byte(bytes[i]); // Is this really how it would work?
        i++;
    }
    free(bytes);

    S3_bar(out);
}

// takes in some bitstream, sums over the product of consecutive bits, check to
// see if this sum is less than one and then constructs either the negation or
// standard version of that polynomial in the S3 ring.
void Ternary_Plus(bitstring_t b, poly *out) {
    assert(b.length == SAMPLE_IID_BITS);
    Ternary(b, out);

    int t = 0;
    for (int i = 0; i <= N - 2; i++) {
        t += out->coeffs[i] * out->coeffs[i + 1];
    }
    int s = t < 0 ? -1 : 1;
    for (int i = 0; i < N - 1; i += 2) {
        out->coeffs[i] *= s;
    }

    S3_bar(out);
}
