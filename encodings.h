#ifndef ENCODINGS_H
#define ENCODINGS_H

#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
// need to include some math functions

// logq = int(log2(c.q)) -> make in C somehow

// converts some string of bits into an array of bit arrays.
void bits_to_bytes(bitstring_t bits, uint8_t *bytes);

// converts some array of bytes into an array of bits.
bitstring_t bytes_to_bits(uint8_t *bytes, size_t n);

// takes in some polynomial a, finds its representation in the Rq ring, and
// extracts all coefficients of post rq representation.
void pack_Rq0(poly *a, uint8_t *result);

// takes in some list of bytes and converts these bytes into integer
// coefficients using some given length of bits / coefficient log2q
void unpack_Rq0(uint8_t *bytes, poly *result);

// takes in some polynomial a, evalutes a in the ring Sq, extracts its
// coefficients, and converts its post sq coefficients to binary with a given
// length per number.
void pack_Sq(poly *a, uint8_t *result);

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length log2(q) and then passes the result to Sq normative
// form.
void unpack_Sq(uint8_t *bytes, poly *result);

// takes in some polynomial a, evalutes a in the ring S3, extracts its post
// transformation coefficients, and converts its post s3  coefficients to binary
// with a given length per number.
void pack_S3(poly *a, uint8_t *result);

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length 8 and then conducts a change of basis operation to
// ternary bytes of length 5. These bytes are used as coefficients of V and
// rerepresented in S3.
poly *unpack_S3(uint8_t *B);

#endif
