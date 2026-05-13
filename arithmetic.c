#include "arithmetic.h"
#include "./tests/arithmetic.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Finds the non-normative representative of the given polynomial
// in the R/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
void Rq(poly *a) {
    for (int i = 0; i < N; i++) {
        a->coeffs[i] = (a->coeffs[i] % Q + Q) % Q;
    }
}

// Finds the canonical representative of the given polynomial
// in the R/q quotient ring
// The difference is that the coefficients of the polynomial
// must be between (-q/2) and (q/2 - 1)
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
void Rq_bar(poly *a) {
    for (int i = 0; i < N; i++) {
        a->coeffs[i] = (a->coeffs[i] % Q + Q) % Q;
        if (a->coeffs[i] >= (Q / 2)) {
            a->coeffs[i] -= Q;
        }
    }
}

// Finds the representative of the given polynomial in the
// S/2 quotient ring
// Note: the non-normative and canonical representation in S/2
// are the same
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(2,Φ_n))
void S2(poly *a) {
    // poly *b = calloc(1, sizeof(poly));
    int last = a->coeffs[N - 1] % 2;
    for (int i = 0; i < N; i++) {
        a->coeffs[i] = ((a->coeffs[i] - last) % 2 + 2) % 2;
    }
}

// Finds the non-normative representative of the given polynomial
// in the S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3(poly *a) {
    int last = a->coeffs[N - 1] % 3;
    for (int i = 0; i < N; i++) {
        a->coeffs[i] = ((a->coeffs[i] - last) % 3 + 3 * 100) % 3;
    }
}

// Finds the canonical representative of the given polynomial
// in the S/3 quotient ring
// The difference is that the coefficients of the polynomial
// must be between -1 and 1
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3_bar(poly *a) {
    S3(a);
    for (int i = 0; i < N; i++) {
        if (a->coeffs[i] > 1)
            a->coeffs[i] -= 3;
    }
}

// Finds the non-normative representative of the given polynomial
// in the S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_n) that is not normative)
void Sq(poly *a) {
    int last = a->coeffs[N - 1] % Q;
    for (int i = 0; i < N; i++) {
        // TODO: Fix this silly way of finding the
        // moudulus when there could be a negative number
        a->coeffs[i] = ((a->coeffs[i] - last + Q * 5) % Q + Q * 5) % Q;
    }
}

// Finds the canonical representative of the given polynomial
// in the S/q quotient ring
// The difference is that the coefficients of the polynomial
// must be between (-q/2) and (q/2 - 1)
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_n) that is canonical)
void Sq_bar(poly *a) {
    int last = a->coeffs[N - 1] % Q;
    for (int i = 0; i < N; i++) {
        a->coeffs[i] = ((a->coeffs[i] - last) % Q + Q) % Q;
        if (a->coeffs[i] > (Q / 2)) {
            a->coeffs[i] -= Q;
        }
    }
}

// Compute polynomial multiplication and modular in R/q
// Ensure that Rq OR Rq_bar is called outside of this
// function to make sure the coefficients are correct
// input: a and b (two polynomials in ring Z[x])
// output: c (polynomial in Z[x]/(q,Φ_1*Φ_n))
void poly_mul_Rq(poly *a, poly *b, poly *out) {
    poly *c = calloc(1, sizeof(poly));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int index = i + j;

            if (index >= N) {
                index -= N;
            }

            int product = a->coeffs[i] * b->coeffs[j];
            c->coeffs[index] = (c->coeffs[index] + product) % Q;
        }
    }

    // Note to calling function: make sure to call either
    // Rq or Rq_bar or the coefficients will be wrong!
    memcpy(out, c, sizeof(poly));
    free(c);
}

void ppoly(short *x, int size) {
    for (int i = 0; i < size; i++) {
        if (!x[i])
            continue;
        printf("%d * x^%d + ", x[i], i);
    }
    printf("\n");
}

void schoolbook(short *x, short *y, int size, short *out) {
    memset(out, 0, sizeof(short) * size * 2);
    for (int i = 0; i < size; i++) {
        if (x[i] == 0)
            continue;
        for (int j = 0; j < size; j++) {
            out[i + j] += x[i] * y[j];
        }
    }
}

void karatsuba(short *x, short *y, int size, short *out) {
    if (size % 2) { // unable to divide polynomials into two even parts
                    // use schoolbook instead
        schoolbook(x, y, size, out);
        return;
    }

    // each is a buffer of size: next_round
    short *x1 = x + size / 2;
    short *x2 = x;
    short *y1 = y + size / 2;
    short *y2 = y;

    short u[size];
    short v[size];
    short w[size];
    karatsuba(x1, y1, size / 2, u);
    karatsuba(x2, y2, size / 2, v);

    short w1[size / 2];
    short w2[size / 2];
    for (int i = 0; i < size / 2; i++) {
        w1[i] = x1[i] - x2[i];
        w2[i] = y1[i] - y2[i];
    }
    karatsuba(w1, w2, size / 2, w);

    memset(out, 0, size * sizeof(short) * 2);
    for (int i = 0; i < size; i++) {
        out[i + size] += u[i];
        out[i + size / 2] += u[i] + v[i] - w[i];
        out[i] += v[i];
    }
}

// Compute polynomial multiplication and modular in S/q
// Ensure that Sq, Sq_bar, S2, S3, OR S3_bar is called
// outside of this function to make sure the coefficients
// are correct
// input: a and b (two polynomials in ring Z[x])
// output: c (polynomial in Z[x]/(q,Φ_n))
void poly_mul_S(poly *a, poly *b, poly *out) {
    short temp[704 * 2];

    // Set extra space in polynomial to 0
    // since its only here as padding
    a->coeffs[701] = 0;
    a->coeffs[702] = 0;
    a->coeffs[703] = 0;
    b->coeffs[701] = 0;
    b->coeffs[702] = 0;
    b->coeffs[703] = 0;
    karatsuba(a->coeffs, b->coeffs, 704, temp);
    for (int i = 0; i < N; i++) {
        out->coeffs[i] = temp[i] + temp[i + N];
    }
    int last = out->coeffs[N - 1];
    for (int i = 0; i < N; i++) {
        out->coeffs[i] = (out->coeffs[i] - last);
    }
}

// Helper function to determine the degree of a polynomial
// input: a (polynomial with a degree less than N)
// output: deg (degree of the polynomial)
int get_degree(poly *a) {
    for (int i = N - 1; i >= 0; i--) {
        if (a->coeffs[i] != 0) {
            return i;
        }
    }
    return -1; // shouldn't reach here?
}

// Compute inverses in S/2 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(2,Φ_n))
void S2_inverse(poly *a, poly *out) {
    S2(a);
    poly b, c;
    memcpy(&b, a, sizeof(poly));

    for (int i = 1; i < N - 2; i++) {
        poly_mul_S(&b, &b, &c);
        S2(&c);
        poly_mul_S(&c, a, &b);
        S2(&b);
    }

    poly_mul_S(&b, &b, &b);
    S2(&b);
    memcpy(out, &b, sizeof(poly));
}

// bitsliced representation of a polynomial with coefficients in {-1, 0, 1}
// in total, there are 12 * 64 = 768 > 700 coefficients each represented with 2
// bits

typedef struct {
    uint64_t b[12]; // 3 256-bit vectors for bottom bits
    uint64_t t[12]; // 3 256-bit vectors for top bits
} bit_poly;

// Method from BITSLICING AND THE METHOD OF FOUR RUSSIANS OVER LARGER FINITE
// FIELDS by Boothby and Bradshaw
void bitsliced_add(uint64_t x0, uint64_t x1, uint64_t y0, uint64_t y1,
                   uint64_t *r_0, uint64_t *r_1) {
    uint64_t s = x0 ^ y1 ^ x1;
    uint64_t t = x1 ^ y0 ^ y1;
    *r_0 = (x0 ^ y1) & (x1 ^ y0);
    *r_1 = s | t;
}

// Method from BITSLICING AND THE METHOD OF FOUR RUSSIANS OVER LARGER FINITE
// FIELDS by Boothby and Bradshaw
void bitsliced_sub(uint64_t x0, uint64_t x1, uint64_t y0, uint64_t y1,
                   uint64_t *r_0, uint64_t *r_1) {
    uint64_t t = x0 ^ y0;
    *r_0 = t | (x1 ^ y1);
    *r_1 = (t ^ y1) & (y0 ^ x1);
}

// Compute inverses in S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3_inverse(poly *a, poly *out) {
    assert(a->coeffs[N - 1] == 0);
    poly original = {.coeffs = {
                         1,
                         1,
                     }};
    // memcpy(&original, a, sizeof(poly));
    S3_bar(&original);

    int delta = 1;
    bit_poly v = {0};

    // r = 1 in S3
    bit_poly r = {.b = {1}};

    // f = x^700 + x^699 + ... + x + 1
    bit_poly f = {0};

    // take original coefficients and bitslice them, but backwards
    bit_poly g = {0};
    for (unsigned short i = 0; i < N - 1; i++) {
        short coeff = original.coeffs[N - 2 - i];
        if (coeff) // matches 1 or -1
            g.b[(i / 256) * 4 + i % 4] |= 1ULL << ((i % 256) / 4);
        if (coeff == -1)
            g.t[(i / 256) * 4 + i % 4] |= 1ULL << ((i % 256) / 4);

        f.b[(i / 256) * 4 + i % 4] |= 1ULL << ((i % 256) / 4); // add x^i to f
    }
    f.b[8] |= 1ULL << 47; // add x^700 to f
                          //

    for (int i = 0; i < 2 * 700 - 1; i++) {
        // Replace v with xv.
        uint64_t top_carry = 0;
        uint64_t bottom_carry = 0;
        for (int j = 0; j < 3; j++) {
            uint64_t temp = v.b[j * 4 + 3];
            v.b[j * 4 + 3] = v.b[j * 4 + 2];
            v.b[j * 4 + 2] = v.b[j * 4 + 1];
            v.b[j * 4 + 1] = v.b[j * 4 + 0];
            v.b[j * 4 + 0] = (temp << 1) | bottom_carry;
            bottom_carry = temp >> 63;

            temp = v.t[j * 4 + 3];
            v.t[j * 4 + 3] = v.t[j * 4 + 2];
            v.t[j * 4 + 2] = v.t[j * 4 + 1];
            v.t[j * 4 + 1] = v.t[j * 4 + 0];
            v.t[j * 4 + 0] = (temp << 1) | top_carry;
            top_carry = temp >> 63;
        }

        // Compute a swap mask as −1 if δ > 0 and g(0) != 0, otherwise 0.
        int swap_mask = (delta > 0 && (g.b[0] & 1ULL)) ? -1 : 0;

        // Compute c ∈ Z/3 as f(0)g(0).
        short f_0 = (f.b[0] & 1ULL) ? ((f.t[0] & 1ULL) ? -1 : 1) : 0;
        short g_0 = (g.b[0] & 1ULL) ? ((g.t[0] & 1ULL) ? -1 : 1) : 0;
        short c = f_0 * g_0;

        // Replace δ with −δ if the swap mask is set.
        if (swap_mask)
            delta = -delta;

        // Add 1 to δ.
        delta++;

        // Replace (f, g) with (g, f ) if the swap mask is set.
        if (swap_mask) {
            bit_poly temp = g;
            g = f;
            f = temp;
        }

        // Replace g with (g − cf)/x.
        for (int j = 0; j < 12; j++) {
            if (c == 1) // subtract
                bitsliced_sub(g.b[j], g.t[j], f.b[j], f.t[j], &g.b[j],
                              &g.t[j]); // g -= f
            else if (c == -1)
                bitsliced_add(g.b[j], g.t[j], f.b[j], f.t[j], &g.b[j],
                              &g.t[j]); // g += f
        }

        top_carry = 0ULL;
        bottom_carry = 0ULL;
        for (int j = 2; j >= 0; j--) { // go backwards for carrying
            uint64_t temp = g.b[j * 4 + 0];

            g.b[j * 4 + 0] = g.b[j * 4 + 1];
            g.b[j * 4 + 1] = g.b[j * 4 + 2];
            g.b[j * 4 + 2] = g.b[j * 4 + 3];
            g.b[j * 4 + 3] = (temp >> 1) | (bottom_carry << 63);
            bottom_carry = temp & 1ULL;

            temp = g.t[j * 4 + 0];
            g.t[j * 4 + 0] = g.t[j * 4 + 1];
            g.t[j * 4 + 1] = g.t[j * 4 + 2];
            g.t[j * 4 + 2] = g.t[j * 4 + 3];
            g.t[j * 4 + 3] = (temp >> 1) | (top_carry << 63);
            top_carry = temp & 1ULL;
        }

        // Replace (v, r) with (r, v) if the swap mask is set.
        if (swap_mask) {
            bit_poly temp = r;
            r = v;
            v = temp;
        }

        // Replace r with r − cv.
        for (int j = 0; j < 12; j++) {
            if (c == 1) // subtract
                bitsliced_sub(r.b[j], r.t[j], v.b[j], v.t[j], &r.b[j],
                              &r.t[j]); // r -= v
            else if (c == -1)
                bitsliced_add(r.b[j], r.t[j], v.b[j], v.t[j], &r.b[j],
                              &r.t[j]); // r += v
        }
    }

    // We thus multiply v by f(0), and take the coefficients of x^0,..., x^699
    // in reverse order, to obtain the desired inverse.
    short f_0 = (f.b[0] & 1) * ((f.t[0] & 1) ? -1 : 1);
    for (unsigned short i = 0; i < N - 1; i++) {
        int idx = (i / 256) * 4 + i % 4;
        int bit = (i % 256) / 4;
        short coeff =
            ((v.b[idx] >> bit) & 1) * (((v.t[idx] >> bit) & 1) ? -1 : 1);
        out->coeffs[N - 2 - i] = coeff * f_0;
    }

    // Final coefficient (x^700) is always 0 in S3
    out->coeffs[N - 1] = 0;

    S3(out);
    S3(&original);
    print_poly("original", &original);
    print_poly("poly inverse: ", out);
    printf("final result %d\n", delta);

    poly test = {0};
    poly_mul_S(out, &original, &test);
    S3(&test);
    print_poly("product %d\n", &test);
    // assert(delta == 1);
    return;
}

// Compute inverses in S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(q,Φ_n))
void Sq_inverse(poly *a, poly *out) {
    poly v0 = {0};
    poly other_a; // other_a is needed since S2_inverse modifies a;
    memcpy(&other_a, a, sizeof(poly));
    S2_inverse(&other_a, &v0);
    /*if (v0 == NULL) {
        return;
    }*/

    S2(&v0);
    int t = 1;
    int stop = (int)log2(Q);

    while (t < stop) {
        // v0 = Sq(v0 * (2 - a * v0))
        // The (2 - a * v0) stage
        poly temp = {0};
        poly_mul_S(a, &v0, &temp);
        Sq(&temp);

        for (int i = 0; i < N; i++) {
            temp.coeffs[i] *= -1;
        }
        temp.coeffs[0] += 2;

        poly_mul_S(&v0, &temp, &v0);
        Sq(&v0);

        t *= 2;
    }

    Sq(&v0);
    memcpy(out, &v0, sizeof(poly));
}

// Maps the given polynomial over a small ternary ring
// Input: m (polynomial)
// Output: (Φ_1 * S3(m/Φ_1)) (polynomial) HRSS
void Lift(poly *m, poly *out) {
    // PHI_1
    poly r0 = {0}, b = {0};
    r0.coeffs[0] = -1;
    r0.coeffs[1] = 1;

    //(PHI_1 * S3_bar(m * S3_inverse(PHI_1)))
    for (int i = 0; i < N - 1; i++) { // Not sure why this works
        switch (i % 3) {
        case 0:
            b.coeffs[i] = 1;
            break;
        case 1:
            b.coeffs[i] = 0;
            break;
        case 2:
            b.coeffs[i] = 2;
            break;
        }
    }

    poly_mul_S(m, &b, &b);
    S3_bar(&b);

    for (int i = 0; i < N - 1; i++) {
        if (b.coeffs[i] == 0) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            out->coeffs[i + j] += (b.coeffs[i] * r0.coeffs[j]);
        }
    }
}
