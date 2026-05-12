#include "encodings.h"
#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// converts some string of bits into an array of bit arrays.
// each bit array is length eight composed of 0 and 1 integers.
// the order follows [b1 b2 b3 b4 b5 b6 b7 b8 b9] into
// [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
void bits_to_bytes(bitstring_t bits, uint8_t *bytes) {
    int byte_count = (bits).length / 8;
    if ((bits).length % 8 != 0)
        byte_count++;
    for (int i = 0; i < byte_count; i++) {
        bytes[i] = flip_byte(bits.data[i]);
    }
}

// converts some array of bytes into an array of bits.
// byte arrays are flipped then appended to an empty bit array
// the order follows [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
// into [b1 b2 b3 b4 b5 b6 b7 b8 b9]
// n is the number of bits
bitstring_t bytes_to_bits(uint8_t *bytes, size_t n) {
    bitstring_t bits = new_bitstring(n);
    int byte_count = n / 8;
    if (n % 8 != 0)
        byte_count++;

    for (int i = 0; i < byte_count; i++) {
        bits.data[i] = flip_byte(bytes[i]);
    }

    return bits;
}

uint8_t flip_byte(uint8_t b) {
    return ((b & 0b1) << 7) | ((b & 0b10) << 5) | ((b & 0b100) << 3) |
           ((b & 0b1000) << 1) | ((b & 0b10000) >> 1) | ((b & 0b100000) >> 3) |
           ((b & 0b1000000) >> 5) | ((b & 0b10000000) >> 7);
}

// takes in some polynomial a, finds its representation in the Rq ring, and
// extracts all coefficients of post rq representation. These coefficients are
// converted to binary and packed into a list for further operation
// Note, this modifies a by applying Rq_bar
void pack_Rq0(poly *a, uint8_t *result) {
    // poly *v = Rq_bar(a);
    Rq_bar(a);
    bitstring_t b = new_bitstring((N - 1) * logQ);
    // TODO: This is very inefficient. Find a better way to set 8 bits at a time
    for (int i = 0; i < (N - 1); i++) {
        int modRes = (a->coeffs[i] + 2 * Q) % Q;

        for (int j = 0; j < logQ; j++) {
            set_nth_bit(b, (i * logQ + j), (modRes & 0x1));
            modRes >>= 1;
        }
    }
    bits_to_bytes(b, result);
    free(b.data);
}

// takes in some list of bytes and converts these bytes into integer
// coefficients using some given length of bits / coefficient log2q
void unpack_Rq0(uint8_t *bytes, poly *out) {
    bitstring_t bits = bytes_to_bits(bytes, ((N - 1) * logQ));
    int total_coeffs = 0;

    // TODO: This is very inefficient. Find a better way to get 8 bits at a time
    for (int i = 0; i < N - 1; i++) {
        int c = 0;
        int temp = 0;
        for (int j = 0; j < logQ; j++) {
            temp = (((int)(get_nth_bit(bits, i * logQ + j))) << j);
            c = c | temp;
            temp = 0;
        }
        out->coeffs[i] = c;
        total_coeffs += c;
    }
    out->coeffs[N - 1] = (Q - total_coeffs) % Q;
    Rq(out);
    free(bits.data);
}

// takes in some polynomial a, evalutes a in the ring Sq, extracts its
// coefficients, and converts its post sq coefficients to binary with a given
// length per number.
void pack_Sq(poly *a, uint8_t *result) {

    Sq_bar(a);
    bitstring_t b = new_bitstring(PACKED_SQ_BYTES * 8);

    // TODO: This is very inefficient. Find a better way to set 8 bits at a time
    for (int i = 0; i < N - 1; i++) {
        int modRes = (a->coeffs[i] + 2 * Q) % Q;

        for (int j = 0; j < logQ; j++) {
            set_nth_bit(b, (i * logQ + j), (modRes & 0x1));
            modRes >>= 1;
        }
    }

    bits_to_bytes(b, result);
    free(b.data);
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length log2(q) and then passes the result to Sq normative
// form.
void unpack_Sq(uint8_t *bytes, poly *out) {
    bitstring_t bits = bytes_to_bits(bytes, ((N - 1) * logQ));

    // TODO: This is very inefficient. Find a better way to get 8 bits at a time
    for (int i = 0; i < N - 1; i++) {
        int c = 0;
        for (int j = 0; j < logQ; j++) {
            c = c | (((int)(get_nth_bit(bits, i * logQ + j))) << j);
        }
        out->coeffs[i] = c;
    }
    Sq(out);
    free(bits.data);
}

// takes in some polynomial a, evalutes a in the ring S3, extracts its post
// transformation coefficients, and converts its post s3  coefficients to binary
// with a given length per number.
void pack_S3(poly *a, uint8_t *result) {
    poly v;
    memcpy(&v, a, sizeof(poly));
    S3_bar(&v);

    for (int i = 0; i < (N - 1) / 5; i++) {
        int c = 0;
        int power = 1;
        for (int j = 0; j < 5; j++) {
            c += power * ((v.coeffs[5 * i + j] + 9) % 3);
            power *= 3;
        }
        assert(c < (2 << 8));
        assert(i < PACKED_S3_BYTES);
        result[i] = c;
    }
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length 8 and then conducts a change of basis operation to
// ternary bytes of length 5. These bytes are used as coefficients of V and
// rerepresented in S3.
void unpack_S3(uint8_t *B, poly *out) {
    assert(out != NULL);
    for (int i = 0; i < (N - 1) / 5; i++) {
        uint8_t byte = B[i];
        for (int j = 0; j < 5; j++) {
            out->coeffs[5 * i + j] = byte % 3;
            byte /= 3;
        }
    }

    S3(out); // TODO: Is this even necessary??
}
