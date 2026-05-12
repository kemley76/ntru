#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "constants.h"
#include <stdlib.h>
#include <string.h>

// Polynomial type
// The polynomials should never go above N - 1 degrees
typedef struct {
    int coeffs[N];
} poly;

typedef struct {
    poly *first;
    poly *second;
} poly_pair;

// TODO: Check to see if these are needed for inverses
// x - 1
// inline constexpr poly PHI_1 = { .coeffs = { [0] = -1, [1] = 1 } };
// (x - 1) / (x^N - 1)
// inline constexpr poly PHI_N = { .coeffs = { [0 ... 700 ] = 1 } };

// Finds the non-normative representative of the given polynomial
// in the R/q quotient ring
void Rq(poly *a);

// Finds the canonical representative of the given polynomial
// in the R/q quotient ring
void Rq_bar(poly *a);

// Finds the representative of the given polynomial in the
// S/2 quotient ring
void S2(poly *a);

// Finds the non-normative representative of the given polynomial
// in the S/3 quotient ring
void S3(poly *a);

// Finds the canonical representative of the given polynomial
// in the S/3 quotient ring
void S3_bar(poly *a);

// Finds the non-normative representative of the given polynomial
// in the S/q quotient ring
void Sq(poly *a);

// Finds the canonical representative of the given polynomial
// in the S/q quotient ring
void Sq_bar(poly *a);

// Compute polynomial multiplication and modular in R/q
// Ensure that Rq OR Rq_bar is called outside of this
// function to make sure the coefficients are correct
void poly_mul_Rq(poly *a, poly *b, poly *out);

// Compute polynomial multiplication and modular in S/q
// Ensure that Sq, Sq_bar, S2, S3, OR S3_bar is called
// outside of this function to make sure the coefficients
// are correct
void poly_mul_S(poly *a, poly *b, poly *out);

// Compute inverses in S/2 quotient ring
void S2_inverse(poly *a, poly *out);

// Compute inverses in S/3 quotient ring
void S3_inverse(poly *a, poly *out);

// Compute inverses in S/q quotient ring
void Sq_inverse(poly *a, poly *out);

// Maps the given polynomial over a small ternary ring
void Lift(poly *m, poly *out);

#endif
