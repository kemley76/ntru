#ifndef SAMPLING_H
#define SAMPLING_H

#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial and another function to create a ternary polynomial with
// non-negative correlation
void Sample_fg(bitstring_t fg_bits, poly *out_f, poly *out_g);

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial from that bitstream
void Sample_rm(bitstring_t rm_bits, poly *out_r, poly *out_m);

// takes in some bitstream, converts it into integers at 8 bits per int
// the passes the list of integers to S3_bar to reduce the result down to the S3
// ring
void Ternary(bitstring_t b, poly *out);

// takes in some bitstream, sums over the product of consecutive bits, check to
// see if this sum is less than one and then constructs either the negation or
// standard version of that polynomial in the S3 ring.
void Ternary_Plus(bitstring_t b, poly *out);

#endif
