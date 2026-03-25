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

// Finds the non-normative representative of the given polynomial
// in the R/q quotient ring
void Rq();

// Finds the canonical representative of the given polynomial
// in the R/q quotient ring
void Rq_bar();

// Finds the representative of the given polynomial in the 
// S/2 quotient ring
void S2();

// Finds the non-normative representative of the given polynomial
// in the S/3 quotient ring
void S3();

// Finds the canonical representative of the given polynomial
// in the S/3 quotient ring
void S3_bar();

// Finds the non-normative representative of the given polynomial
// in the S/q quotient ring
void Sq();

// Finds the canonical representative of the given polynomial
// in the S/q quotient ring
void Sq_bar();

// Compute inverses in S/2 quotient ring
void S2_inverse();

// Compute inverses in S/3 quotient ring
void S3_inverse();

// Compute inverses in S/q quotient ring
void Sq_inverse();

// Maps the given polynomial over a small ternary ring
void Lift();

#endif