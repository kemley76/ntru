#include "encodings.h"
#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// converts some string of bits into an array of bit arrays.
// each bit array is length eight composed of 0 and 1 integers.
// the order follows [b1 b2 b3 b4 b5 b6 b7 b8 b9] into
// [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
void bits_to_bytes(bitstring_t bits, uint8_t *bytes) {
    int byte_count = bits.length / 8;
    if (bits.length % 8 != 0)
        byte_count++;
    for (int i = 0; i < byte_count; i++) {
        uint8_t r = bits.data[i]; // raw bits (lsb first)
                                  // flip the bits to have msb first
        bytes[i] = ((r & 0b1) << 7) | ((r & 0b10) << 5) | ((r & 0b100) << 3) |
                   ((r & 0b1000) << 1) | ((r & 0b10000) >> 1) |
                   ((r & 0b100000) >> 3) | ((r & 0b1000000) >> 5) |
                   ((r & 0b10000000) >> 7);
    }
}

// converts some array of bytes into an array of bits.
// byte arrays are flipped then appended to an empty bit array
// the order follows [[b8 b7 b6 b5 b4 b3 b2 b1] [0 0 0 0 0 0 0 b9]]
// into [b1 b2 b3 b4 b5 b6 b7 b8 b9]
// n is the number of bits
bitstring_t bytes_to_bits(uint8_t *bytes, size_t n) {
    bitstring_t bits = new_bistring(n);
    int byte_count = n / 8;
    if (n % 8 != 0)
        byte_count++;

    for (int i = 0; i < byte_count; i++) {
        uint8_t r = bytes[i];
        bits.data[i] = ((r & 0b1) << 7) | ((r & 0b10) << 5) |
                       ((r & 0b100) << 3) | ((r & 0b1000) << 1) |
                       ((r & 0b10000) >> 1) | ((r & 0b100000) >> 3) |
                       ((r & 0b1000000) >> 5) | ((r & 0b10000000) >> 7);
    }

    return bits;
}

// takes in some polynomial a, finds its representation in the Rq ring, and
// extracts all coefficients of post rq representation. These coefficients are
// converted to binary and packed into a list for further operation
void pack_Rq0(poly* a, uint8_t* result ) {
    // fprintf(stderr, "Error: Function is not implemented.\n");
    // exit(EXIT_FAILURE);
    poly* v;
    v = Rq_bar(a);
    bitstring_t b = new_bistring((N-1)*logQ);
    for (int i = 0 ; i < (N - 1); i++) {
        int modRes = v->coeffs[i] % Q;
        int targetByte = (i * logQ) / 8;
        int targetBit = (i *logQ) % 8;
        // A the modular division result of a coefficient of v by Q takes log2(Q) (which is 13 in our case) bits to represent. 13 is a yucky number in binary
        // thanks to the super duper special property of being prime. For memory space's sake we cram a bitstream into
        // bytes. So we need a clean way to break up a 13 bit number into pieces so that it fits into the tail and starting 
        // end of bitstream's byte array. Consider the first two bytes of data in bitstream. We'll suppose that we get the coefficient
        // 1 1011 0001 1001 and the coefficient 0 1101 0011 1100. The first byte we write into data is just the eight most significant bits of the
        // first coefficient: 1101 1000. The second byte we write is the remaining five bits of the coefficient left shifted to become the most significant
        // bits of a byte: 1100 1000. 
        // When we want to write the second coefficient to bitstream we run into a bit more of a problem. We can't just write the eight most significant bits because
        // we'd overwrite the prior contents of the second byte of datastream. Instead we need to write just the three most significant bits of the coefficient
        // to the three least significant bits of datastream byte. We get: 1100 1011 from the prior bitstream byte 1100 1000 and the 3 MSBs of current coefficient 0 1101 0011 1100.
        // Now we can start to fill the third byte of datastream from the eight bits after the three MSBs of the coefficient. That follows immediately as  0100 1111. 
        // Finally we fill the third byte using the last two bits of the second coefficient as the most significant bits of the datastream byte: 0000 0000. 

        // So what rules made this nonsense work? First we need to know which byte we're supposed to work with from the start. We can calculate this value as the number of bits which
        // have already been written divided by eight: effectively that's log2(Q) times the number of loops divided by eight. Next we need to know which bits have already been 
        // written to by a prior iteration of this loop. We can find this value by calculating the remainder of the number of bits that have already been written divided by eight. That's just 
        // i * logQ % 8. We can accomplish our first bit write as the 8-mod result most significant bits of the coefficient as the least significant bits of the datastream byte: this is coefficient >> 13-(i*log2Q % 8).
        // The second bit write is either the remaining or the following eight. In the case of remaining it's coefficient << (8 - (13 - i*logQ2 % 8)) masked with 0xFF. In the case of the following eight we right shift by
        // five minus the bits from last stage which is coefficient >> 5-(8-(log2Q % 8)). Finally in a following eight case we have to account for the remainder as 
        // the most significant bits of a byte which is coefficient << (5-(8-targetBit))). 

        int8_t seperatePoint = 13 - targetBit ;

        if (targetBit + 13 <= 16) {
            b.data[targetByte] = (b.data[targetByte] | (modRes >> seperatePoint));
            b.data[targetByte + 1] = modRes << (8 - seperatePoint);
        } else {
            b.data[targetByte] = (b.data[targetByte] | (modRes >> seperatePoint));
            b.data[targetByte + 1] = (modRes >> (5-(8-targetBit)));
            b.data[targetByte + 2] = (modRes << (8-(5-(8-targetBit))));
        }
    }
    bits_to_bytes(b, result);
    // TODO test this function. I have less than zero confidence in it - Thomas
}


// takes in some list of bytes and converts these bytes into integer
// coefficients using some given length of bits / coefficient log2q
void unpack_Rq0(uint8_t* bytes, poly* result) {
    // fprintf(stderr, "Error: Function is not implemented.\n");
    // exit(EXIT_FAILURE);
    bitstring_t bits = bytes_to_bits(bytes, ((N-1)*logQ) );
    poly V = { 0 };
    for (int i =0 ; i < N ; i++) {
        int c = 0;
        for (int j = 0 ; j < logQ ; j ++) {
            c = c | (((int)(get_nth_bit(bits, i*logQ+j))) << j);
        }
        V.coeffs[i] = c;
        V.coeffs[N-1] =  V.coeffs[N-1] - c;
    }
    result = Rq(&V);
}


// takes in some polynomial a, evalutes a in the ring Sq, extracts its
// coefficients, and converts its post sq coefficients to binary with a given
// length per number.
void pack_Sq(poly* a, uint8_t* result) {
    // fprintf(stderr, "Error: Function is not implemented.\n");
    // exit(EXIT_FAILURE);

    poly *V = Sq_bar(a);
    bitstring_t b = new_bistring((N-1)*logQ);
    for (int i = 0 ; i < (N - 1); i++) {
        int modRes = V->coeffs[i] % Q;
        int targetByte = (i * logQ) / 8;
        int targetBit = (i *logQ) % 8;
        int8_t seperatePoint = 13 - targetBit ;

        if (targetBit + 13 <= 16) {
            b.data[targetByte] = (b.data[targetByte] | (modRes >> seperatePoint));
            b.data[targetByte + 1] = modRes << (8 - seperatePoint);
        } else {
            b.data[targetByte] = (b.data[targetByte] | (modRes >> seperatePoint));
            b.data[targetByte + 1] = (modRes >> (5-(8-targetBit)));
            b.data[targetByte + 2] = (modRes << (8-(5-(8-targetBit))));
        }
    }
    bits_to_bytes(b, result);
}

// takes in some list of bytes, forms a list of bits, breaks up the list of bits
// into sections of length log2(q) and then passes the result to Sq normative
// form.
void unpack_Sq(uint8_t* bytes, poly* result) {
    // fprintf(stderr, "Error: Function is not implemented.\n");
    // exit(EXIT_FAILURE);
    bitstring_t bits = bytes_to_bits(bytes, ((N-1)*logQ) );
    poly V = { 0 };
    for (int i =0 ; i < N ; i++) {
        int c = 0;
        for (int j = 0 ; j < logQ ; j ++) {
            c = c | (((int)(get_nth_bit(bits, i*logQ+j))) << j);
        }
        V.coeffs[i] = c;
    }
    result = Sq(&V);
}

// takes in some polynomial a, evalutes a in the ring S3, extracts its post
// transformation coefficients, and converts its post s3  coefficients to binary
// with a given length per number.
void pack_S3(poly *a, uint8_t *result) {
    poly *v = S3_bar(a);

    for (int i = 0; i < (N - 1) / 5; i++) {
        int c = 0;
        int power = 1;
        for (int j = 0; j < 5; j++) {
            c += power * ((v->coeffs[5 * i + j] + 9) % 3);
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
poly *unpack_S3(uint8_t *B) {
    poly *v = malloc(sizeof(poly));
    assert(v != NULL);
    for (int i = 0; i < (N - 1) / 5; i++) {
        uint8_t byte = B[i];
        for (int j = 0; j < 5; j++) {
            v->coeffs[5 * i + j] = byte % 3;
            byte /= 3;
        }
    }

    return S3(v);
}
