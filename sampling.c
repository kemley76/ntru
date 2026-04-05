#include "sampling.h"
#include "constants.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial and another function to create a ternary polynomial with
// non-negative correlation
void Sample_fg(bitstring_t fg_bits) {
    bitstring_pair_t pair = split_at(fg_bits, SAMPLE_IID_BITS);
    bitstring_t f_bits = pair.front;
    bitstring_t g_bits = pair.back;

    // fix when polynomial type is added
    // Polynomial f = Ternary_Plus(f_bits)
    // Polynomial g_0 = Ternary_Plus(g_bits)

    // might be different interface
    // Polynomial g = mod_mul(PHI_1, g_0);
    // return (poly_pair){.fst = f, .snd = g} // figure out how to do this best
    // (polynomial pair struct?)

    fprintf(stderr, "Error: Function is not finished yet.\n");
    exit(EXIT_FAILURE);
}

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial from that bitstream
void Sample_rm(bitstring_t rm_bits) {
    bitstring_pair_t pair = split_at(rm_bits, SAMPLE_IID_BITS);
    bitstring_t r_bits = pair.front;
    bitstring_t m_bits = pair.back;

    // fix when polynomial type is added
    // Polynomial r = Ternary(r_bits)
    // Polynomial m = Ternary(m_bits)

    // return (poly_pair){.fst = r, .snd = m} // figure out how to do this best
    // (polynomial pair struct?)

    fprintf(stderr, "Error: Function is not finished yet.\n");
    exit(EXIT_FAILURE);
}

// takes in some bitstream, converts it into integers at 8 bits per int
// the passes the list of integers to S3_bar to reduce the result down to the S3
// ring
void Ternary(bitstring_t b) {
    assert(b.length == SAMPLE_IID_BITS);
    // Polynomial p = new_polynomial(SAMPLE_IID_BITS / 8); // that many
    // coefficients bits_to_bytes(b, p->coeffs);
    // Polynomial v = 0;

    // Original algorithm: This seems like a complicated way of just copying the
    // individual bits into the coefficients??
    /*int i = 0;
    while (i < N) {
            // v.coeffs[i] = b.data[i];	 // Is this really how it would work?
            i++;
    }*/

    // return S3_bar(v);

    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some bitstream, sums over the product of consecutive bits, check to
// see if this sum is less than one and then constructs either the negation or
// standard version of that polynomial in the S3 ring.
void Ternary_Plus(bitstring_t b) {
    assert(b.length == SAMPLE_IID_BITS);

    // Polynomial v = Ternary(b);
    int t = 0;
    for (int i = 0; i < N - 2; i++) {
        // t += v.coeffs[i] + 1 * v.coeffs[i + 1];
    }
    int s = t < 0 ? -1 : 1;
    int i = 0;
    while (i < N - 1) {
        // v.coeffs[i] *= s;
        i += 2;
    }

    // return S3_bar(v)

    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// put comment here
void correlation() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}
