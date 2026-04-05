#include "kat.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// Parses the KAT file and checks and times our implementation against the tests
// contained in it
void test_all() {
    FILE *fptr = fopen("PQCkemKAT_1450.rsp", "r");
    fscanf(fptr, "%*[^\n]\n"); // skips to the next line

    char seed[150];
    char pk[2300];
    char sk[3000];
    char ct[2300];
    char ss[100];
    for (int i = 0; i < 100; i++) {
        fscanf(fptr, "%*[^\n]\n"); // skip count = X

        fscanf(fptr, "seed = %s\n", seed);
        fscanf(fptr, "pk = %s\n", pk);
        fscanf(fptr, "sk = %s\n", sk);
        fscanf(fptr, "ct = %s\n", ct);
        fscanf(fptr, "ss = %s\n", ss);
        printf("\n\ncount = %d\nseed: %s\nss: %s\n", i, seed, ss);
    }
}

void test_kat() {
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}
