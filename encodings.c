#include "encodings.h"

// converts some string of bits into an array of bit arrays. 
// each bit array is length eight composed of 0 and 1 integers.
// the order follows [b1 b2 b3 b4 b5 b6 b7 b8 b9] into 
// [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
void bits_to_bytes(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// converts some array of bytes into an array of bits.
// byte arrays are flipped then appended to an empty bit array
// the order follows [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]] 
// into [b1 b2 b3 b4 b5 b6 b7 b8 b9]
void bytes_to_bits(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some polynomial a, finds its representation in the Rq ring, and extracts all coefficients of post rq representation.
// These coefficients are converted to binary and packed into a list for further operation
void pack_Rq0(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some list of bytes and converts these bytes into integer coefficients
// using some given length of bits / coefficient log2q
void unpack_Rq0(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some polynomial a, evalutes a in the ring Sq, extracts its coefficients, and converts its post sq
// coefficients to binary with a given length per number.
void pack_Sq(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits into sections of length
// log2(q) and then passes the result to Sq normative form. 
void unpack_Sq(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some polynomial a, evalutes a in the ring S3, extracts its post transformation coefficients, 
// and converts its post s3  coefficients to binary with a given length per number. 
void pack_S3(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits into sections of length
// 8 and then conducts a change of basis operation to ternary bytes of length 5. These bytes are used as
// coefficients of V and rerepresented in S3.
void unpack_S3(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}