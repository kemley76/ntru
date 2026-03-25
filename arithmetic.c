#include "arithmetic.h"

// Finds the non-normative representative of the given polynomial
// in the R/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
void Rq(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Finds the canonical representative of the given polynomial
// in the R/q quotient ring
// The difference is that the coefficients of the polynomial
// must be between (-q/2) and (q/2 - 1)
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_1*Φ_n))
void Rq_bar(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Finds the representative of the given polynomial in the 
// S/2 quotient ring
// Note: the non-normative and canonical representation in S/2
// are the same
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(2,Φ_n))
void S2(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Finds the non-normative representative of the given polynomial
// in the S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Finds the canonical representative of the given polynomial
// in the S/3 quotient ring
// The difference is that the coefficients of the polynomial
// must be between -1 and 1
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3_bar(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Finds the non-normative representative of the given polynomial
// in the S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_n) that is not normative)
void Sq(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Finds the canonical representative of the given polynomial
// in the S/q quotient ring
// The difference is that the coefficients of the polynomial
// must be between (-q/2) and (q/2 - 1)
// input: a (polynomial in ring Z[x])
// output: b (polynomial in ring Z[x]/(q,Φ_n) that is canonical)
void Sq_bar(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Compute inverses in S/2 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(2,Φ_n))
void S2_inverse(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Compute inverses in S/3 quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(3,Φ_n))
void S3_inverse(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Compute inverses in S/q quotient ring
// input: a (polynomial in ring Z[x])
// output: b (polynomial in Z[x]/(q,Φ_n))
void Sq_inverse(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Maps the given polynomial over a small ternary ring
// Input: m (polynomial)
// Output: (Φ_1 * S3(m/Φ_1)) (polynomial) HRSS
void Lift(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}