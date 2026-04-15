// testing file, like we had in sage

#include "./tests/encodings.h"
#include "./tests/utils.h"
#include "./tests/kat.h"
#include "arithmetic.h"
#include <stdint.h>

void print_poly(const char *name, poly *a) {
    printf("%s: ", name);
    for (int i = N; i >= 0; i--) {
        if (a->coeffs[i]){
            printf("%dx^%d + ", a->coeffs[i], i);
        }
    }
    printf("\n");
}

int main(int argc, char **argv) {
    poly *a = calloc(1, sizeof(poly));
    // *a = (poly){0}; 
    a->coeffs[100] = 1;
    a->coeffs[1] = 5000;
    a->coeffs[0] = -5;
    poly *b = calloc(1, sizeof(poly));
    b->coeffs[3] = 1;
    b->coeffs[2] = 5;
    b->coeffs[1] = -3000;
    b->coeffs[0] = -5;

    // for (int i = 0; i < N; i++) a->coeffs[i] = i + 10;
    print_poly("Original a", a);
    print_poly("Original b", b);
    
    poly *c = Sq_inverse(b);
    // c = S3(c);
    print_poly("Sq Result", c); // Should be 10%Q, 11%Q...
    
    // 6. Test Case Example: S2 Inverse (Requires specific input)
    // Add tests for S2_inverse, poly_mul_S, etc.

    free(a);
    free(b);
    return 0;
    // test_all_encodings();
    // test_all_utils();
}