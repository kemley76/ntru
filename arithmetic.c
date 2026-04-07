#include "arithmetic.h"
#include <stdio.h>
#include <stdlib.h>

// Finds the non-normative representative of the given polynomial
// in the R/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
poly *Rq(poly *a) {
    poly *b = malloc(sizeof(poly));
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = a->coeffs[i] % Q;
    }
    return b;
}

// Finds the canonical representative of the given polynomial
// in the R/q quotient ring
// The difference is that the coefficients of the polynomial
// must be between (-q/2) and (q/2 - 1)
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
poly *Rq_bar(poly *a) {
    poly *b = malloc(sizeof(poly));
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = a->coeffs[i] % Q;
        if (b->coeffs[i] >= (Q / 2)) {
            b->coeffs[i] -= Q;
        }
    }
    return b;
}

// Finds the representative of the given polynomial in the
// S/2 quotient ring
// Note: the non-normative and canonical representation in S/2
// are the same
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(2,Φ_n))
poly *S2(poly *a) {
    poly *b = malloc(sizeof(poly));
    int last =
        a->coeffs[N - 1] % 2; // TODO: Check to make sure this is how it works
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = (a->coeffs[i] - last) % 2;
    }
    return b;
}

// Finds the non-normative representative of the given polynomial
// in the S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
poly *S3(poly *a) {
    poly *b = malloc(sizeof(poly));
    int last = a->coeffs[N - 1] % 3;
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = (a->coeffs[i] - last) % 3;
    }
    return b;
}

// Finds the canonical representative of the given polynomial
// in the S/3 quotient ring
// The difference is that the coefficients of the polynomial
// must be between -1 and 1
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
poly *S3_bar(poly *a) {
    poly *b = malloc(sizeof(poly));
    int last = a->coeffs[N - 1] % 3;
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = (a->coeffs[i] - last) % 3;
        if (b->coeffs[i] > 1) {
            b->coeffs[i] -= 3;
        }
    }
    return b;
}

// Finds the non-normative representative of the given polynomial
// in the S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_n) that is not normative)
poly *Sq(poly *a) {
    poly *b = malloc(sizeof(poly));
    int last = a->coeffs[N - 1] % Q;
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = (a->coeffs[i] - last) % Q;
    }
    return b;
}

// Finds the canonical representative of the given polynomial
// in the S/q quotient ring
// The difference is that the coefficients of the polynomial
// must be between (-q/2) and (q/2 - 1)
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_n) that is canonical)
poly *Sq_bar(poly *a) {
    poly *b = malloc(sizeof(poly));
    int last = a->coeffs[N - 1] % Q;
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = (a->coeffs[i] - last) % Q;
        if (b->coeffs[i] > (Q / 2)) {
            b->coeffs[i] -= Q;
        }
    }
    return b;
}

// Compute inverses in S/2 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(2,Φ_n))
void S2_inverse() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Compute inverses in S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3_inverse() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Compute inverses in S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(q,Φ_n))
void Sq_inverse() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Maps the given polynomial over a small ternary ring
// Input: m (polynomial)
// Output: (Φ_1 * S3(m/Φ_1)) (polynomial) HRSS
void Lift() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}
