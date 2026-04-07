#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "constants.h"

// From the original definitions:
// create polynomial rings, quotient rings, and phi_1 and phi_n
// Z = ... -> int ring
// PHI_1 = x - 1
// PHI_N = ... -> phi_n equation
// R = ... -> quotient ring using Z and PHI_1*PHI_N
// S = ... -> quotient ring using Z and PHI_N

// Polynomial type
// The polynomials should never go above N - 1 degrees
// Note: find out if the polynomials in other functions
//      will go over this amount...
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

// Compute inverses in S/2 quotient ring
void S2_inverse();

// Compute inverses in S/3 quotient ring
void S3_inverse();

// Compute inverses in S/q quotient ring
void Sq_inverse();

// Maps the given polynomial over a small ternary ring
void Lift();

#endif
