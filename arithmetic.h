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
poly *Rq(poly *a);

// Finds the canonical representative of the given polynomial
// in the R/q quotient ring
poly *Rq_bar(poly *a);

// Finds the representative of the given polynomial in the
// S/2 quotient ring
poly *S2(poly *a);

// Finds the non-normative representative of the given polynomial
// in the S/3 quotient ring
poly *S3(poly *a);

// Finds the canonical representative of the given polynomial
// in the S/3 quotient ring
poly *S3_bar(poly *a);

// Finds the non-normative representative of the given polynomial
// in the S/q quotient ring
poly *Sq(poly *a);

// Finds the canonical representative of the given polynomial
// in the S/q quotient ring
poly *Sq_bar(poly *a);

// Compute polynomial multiplication and modular in R/q
// Ensure that Rq OR Rq_bar is called outside of this 
// function to make sure the coefficients are correct
poly *poly_mul_Rq(poly *a, poly *b);

// Compute polynomial multiplication and modular in S/q
// Ensure that Sq, Sq_bar, S2, S3, OR S3_bar is called 
// outside of this function to make sure the coefficients 
// are correct
poly *poly_mul_S(poly *a, poly *b);

// Compute inverses in S/2 quotient ring
poly *S2_inverse(poly *a);

// Compute inverses in S/3 quotient ring
poly *S3_inverse(poly *a);

// Compute inverses in S/q quotient ring
poly *Sq_inverse(poly *a);

// Maps the given polynomial over a small ternary ring
poly *Lift(poly *m);

#endif
