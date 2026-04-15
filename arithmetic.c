#include "arithmetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Finds the non-normative representative of the given polynomial
// in the R/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
poly *Rq(poly *a) {
    poly *b = calloc(1, sizeof(poly));
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = (a->coeffs[i] % Q + Q) % Q;
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
        b->coeffs[i] = (a->coeffs[i] % Q + Q) % Q;
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
    int last = a->coeffs[N - 1] % 2; // TODO: Check to make sure this is how it works
    for (int i = 0; i < N; i++) {
        b->coeffs[i] = ((a->coeffs[i] - last) % 2 + 2) % 2;
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
        b->coeffs[i] = ((a->coeffs[i] - last) % 3 + 3) % 3;
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
        b->coeffs[i] = ((a->coeffs[i] - last) % 3 + 3) % 3;
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
        b->coeffs[i] = ((a->coeffs[i] - last) % Q + Q) % Q;
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
        b->coeffs[i] = ((a->coeffs[i] - last) % Q + Q) % Q;
        if (b->coeffs[i] > (Q / 2)) {
            b->coeffs[i] -= Q;
        }
    }
    return b;
}

// Compute polynomial multiplication and modular in R/q
// Ensure that Rq OR Rq_bar is called outside of this 
// function to make sure the coefficients are correct
// input: a and b (two polynomials in ring Z[x])
// output: c (polynomial in Z[x]/(q,Φ_1*Φ_n))
poly *poly_mul_Rq(poly *a, poly *b) {
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
    return c;
}

// Compute polynomial multiplication and modular in S/q
// Ensure that Sq, Sq_bar, S2, S3, OR S3_bar is called 
// outside of this function to make sure the coefficients 
// are correct
// input: a and b (two polynomials in ring Z[x])
// output: c (polynomial in Z[x]/(q,Φ_n))
poly *poly_mul_S(poly *a, poly *b) {
    poly *c = calloc(1, sizeof(poly));

    for (int i = 0; i < N; i++) {
        if (a->coeffs[i] == 0) {
            continue;
        }
        for (int j = 0; j < N; j++) {
            int index = i + j;

            if (index >= N) {
                index -= N;
            }

            // int product = a->coeffs[i] * b->coeffs[j];
            c->coeffs[index] = (c->coeffs[index] + a->coeffs[i] * b->coeffs[j]);
        }
    }

    int last = c->coeffs[N-1];
    for (int i = 0; i < N; i++) {
        c->coeffs[i] = (c->coeffs[i] - last);
    }

    // Note to calling function: make sure to call the 
    // other S functions or the coefficients will be wrong!
    return c;
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
poly *S2_inverse(poly *a) {
    // PHI_N
    poly r0;
    for (int i = 0; i < N; i++) {
        r0.coeffs[i] = 1;
    }
    poly r1 = *a;
    r1 = *S2(&r1);
    poly *s0 = calloc(1, sizeof(poly));
    poly *s1 = calloc(1, sizeof(poly));
    s1->coeffs[0] = 1;

    int deg_r0 = get_degree(&r0);
    int deg_r1 = get_degree(&r1);
    while (deg_r1 != -1){
        if (deg_r0 < deg_r1) {
            poly tempr = r0;
            r0 = r1;
            r1 = tempr;
            poly *temps = s0;
            s0 = s1;
            s1 = temps;
            int tempdeg = deg_r0;
            deg_r0 = deg_r1;
            deg_r1 = tempdeg;
        }

        int shift = deg_r0 - deg_r1;
        for (int i = 0; i <= deg_r1; i++) {
            r0.coeffs[i + shift] ^= r1.coeffs[i];
        }

        for (int i = 0; i < N - shift; i++) {
            s0->coeffs[i + shift] ^= s1->coeffs[i];
        }
        deg_r0 = get_degree(&r0);
        deg_r1 = get_degree(&r1);
    }

    if (deg_r0 == 0 && (r0.coeffs[0] & 1)) {
        if (s0->coeffs[N-1] == 1) {
            for (int i = 0; i < N; i++) s0->coeffs[i] ^= 1;
        }
        free(s1);
        return s0;
    }

    free(s0);
    free(s1);
    return NULL;

}

// Compute inverses in S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
poly *S3_inverse(poly *a) {
    // PHI_N
    poly r0;
    for (int i = 0; i < N; i++) {
        r0.coeffs[i] = 1;
    }

    poly r1 = *a;
    r1 = *S3(&r1);

    poly *s0 = calloc(1, sizeof(poly));
    poly *s1 = calloc(1, sizeof(poly));
    s1->coeffs[0] = 1;

    int deg_r0 = get_degree(&r0);
    int deg_r1 = get_degree(&r1);

    while (deg_r1 != -1){
        if (deg_r0 < deg_r1) {
            poly tempr = r0;
            r0 = r1;
            r1 = tempr;
            poly *temps = s0;
            s0 = s1;
            s1 = temps;
            int tempdeg = deg_r0;
            deg_r0 = deg_r1;
            deg_r1 = tempdeg;
        }

        int shift = deg_r0 - deg_r1;

        // Add 1 or add 2 (which is subtract 1)
        int factor = (r0.coeffs[deg_r0] * r1.coeffs[deg_r1]) % 3;

        for (int i = 0; i <= deg_r1; i++) {
            r0.coeffs[i + shift] = (r0.coeffs[i + shift] - factor * r1.coeffs[i] + 9) % 3;
        }

        for (int i = 0; i < N; i++) {
            if (s1->coeffs[i] != 0) {
                int target_idx = (i + shift) % N;
                s0->coeffs[target_idx] = (s0->coeffs[target_idx] - factor * s1->coeffs[i] + 9) % 3;
            }
        }

        deg_r0 = get_degree(&r0);
        deg_r1 = get_degree(&r1);
    }

    if (deg_r0 == 0 && r0.coeffs[0] != 0) {
        if (r0.coeffs[0] == 2) {
            for (int i = 0; i < N; i++) { 
                s0->coeffs[i] = (s0->coeffs[i] * 2) % 3;
            }
        }
        int last = s0->coeffs[N-1];
        if (last != 0) {
            for (int i = 0; i < N; i++) {
                s0->coeffs[i] = (s0->coeffs[i] - last + 9) % 3;
            }
        }
        free(s1);
        return s0;
    }

    free(s0);
    free(s1);
    return NULL;
}

// Compute inverses in S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(q,Φ_n))
poly *Sq_inverse(poly *a) {
    poly *check = S2_inverse(a);
    if (check == NULL) {
        return NULL;
    }
    poly *v0 = S2(check);
    free(check);
    int t = 1;
    int stop = (int)log2(Q);

    while (t < 2) {
        // v0 = Sq(v0 * (2 - a * v0))
        // The (2 - a * v0) stage
        poly *temp = Sq(poly_mul_S(a, v0));
        for (int i = 0; i < N; i++) {
            temp->coeffs[i] *= -1;
        }
        temp->coeffs[0] += 2;

        v0 = Sq(poly_mul_Rq(v0, temp));
        t *= 2;
        free(temp);
    }

    return v0;
}

// Maps the given polynomial over a small ternary ring
// Input: m (polynomial)
// Output: (Φ_1 * S3(m/Φ_1)) (polynomial) HRSS
poly *Lift(poly *m) {
    // PHI_1
    poly *r0 = calloc(1, sizeof(poly));
    r0->coeffs[0] = -1;
    r0->coeffs[1] = 1;

    //(PHI_1 * S3_bar(m * S3_inverse(PHI_1)))
    poly *b = S3_inverse(r0);
    b = poly_mul_S(m, b);
    b = S3_bar(b);

    poly *c = calloc(1, sizeof(poly));

    for (int i = 0; i < N - 1; i++) {
        if (b->coeffs[i] == 0) {
            continue;
        }
        for (int j = 0; j < 2; j++) {
            c->coeffs[i + j] += (b->coeffs[i] * r0->coeffs[j]);
        }
    }
    free(r0);
    free(b);
    return c;
}
