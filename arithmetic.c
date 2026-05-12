#include "arithmetic.h"
#include "./tests/arithmetic.h"
#include <assert.h>
#include <math.h>
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

// Compute inverses in S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3_inverse(poly *a, poly *out) {
    poly b;
    poly c = {0};

    // initialize b = S3(a)
    poly other_a;
    memcpy(&other_a, a, sizeof(poly));
    S3(&other_a);
    memcpy(&b, &other_a, sizeof(poly));

    for (int i = 1; i < N - 2; i++) {
        // printf("%d/%d\n", i, N - 2);
        //  c = b ^ 3
        poly_mul_S(&b, &b, &c);
        poly_mul_S(&c, &b, &c);
        S3(&c);
        poly_mul_S(&c, &other_a, &b);
        S3(&b);
    }

    // b = b ^ 3
    poly temp = {0};
    poly_mul_S(&b, &b, &temp);
    poly_mul_S(&temp, &b, &b);
    S3(&b);

    // TODO: This seems to be an expensive check. Fix!
    poly check = {0};
    poly_mul_S(&other_a, &b, &check);
    S3(&check);
    if (check.coeffs[0] == 2) {
        for (int i = 0; i < N; i++)
            b.coeffs[i] = (b.coeffs[i] * 2) % 3;
    }

    memcpy(out, &b, sizeof(poly));
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
