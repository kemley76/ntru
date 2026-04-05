#include "sampling.h"
#include <stdio.h>
#include <stdlib.h>

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial and another function to create a ternary polynomial with
// non-negative correlation
void Sample_fg() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// splits up the given bitstream of length twice sample iid bits into f and g
// bits then passes each bitstream into a function to create a ternary
// polynomial from that bitstream
void Sample_rm() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some bitstream, converts it into integers at 8 bits per int
// the passes the list of integers to S3_bar to reduce the result down to the S3
// ring
void Ternary() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some bitstream, sums over the product of consecutive bits, check to
// see if this sum is less than one and then constructs either the negation or
// standard version of that polynomial in the S3 ring.
void Ternary_Plus() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// put comment here
void correlation() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}
