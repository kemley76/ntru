// testing file, like we had in sage

#include "./tests/arithmetic.h"
#include "./tests/encodings.h"
#include "./tests/kat.h"
#include "./tests/utils.h"
#include "arithmetic.h"
#include <stdint.h>

void print_poly(const char *name, poly *a) {
    printf("%s: ", name);
    for (int i = N; i >= 0; i--) {
        if (a->coeffs[i]) {
            printf("%dx^%d + ", a->coeffs[i], i);
        }
    }
    printf("\n");
}

int main(int argc, char **argv) {
    // test_all_encodings();
    // test_all_utils();
    test_all_arithmetic();
    // test_all_kat();
}
