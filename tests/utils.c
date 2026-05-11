#include "../utils.h"
#include "../encodings.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *input_hex =
    "0615502300158C5EC95595FE04EF7A25767F2E24"; // 40 hex digits long
uint8_t expected_output[20] = {0x06, 0x15, 0x50, 0x23, 0x00, 0x15, 0x8C,
                               0x5E, 0xC9, 0x55, 0x95, 0xFE, 0x04, 0xEF,
                               0x7A, 0x25, 0x76, 0x7F, 0x2E, 0x24};

// return 1 for pass, 0 for fail
int test_all_utils() {
    int all_pass = 1;
    all_pass &= test_bytes_to_from_hex();
    all_pass &= test_hash();
    if (all_pass)
        printf("All util tests passed\n");
    else
        printf("Util test failed\n");
    return 1;
}

int test_bytes_to_from_hex() {
    // test the hex_to_bytes and bytes_to_hex
    uint8_t output_bytes[20];
    hex_to_bytes(input_hex, 20, output_bytes); // works

    // ensure the bytes match the expected output
    for (int i = 0; i < 20; i++) {
        if (output_bytes[i] != expected_output[i]) {
            printf("test_bytes_to_from_hex: output %X != expected %X after "
                   "hex_to_bytes\n",
                   output_bytes[i], expected_output[i]);
            return 0;
        }
    }

    char output_hex[41];
    bytes_to_hex(output_bytes, 20, output_hex);
    if (strncmp(input_hex, output_hex, 40)) { // strings don't match
        printf("test_bytes_to_from_hex: output %s != expected %s\n", output_hex,
               input_hex);
        return 0;
    }
    printf("test_bytes_to_from_hex: test passed\n");
    return 1;
}

uint8_t data[20] = {0x06, 0x15, 0x50, 0x23, 0x4D, 0x15, 0x8C, 0x5E, 0xC9, 0x55,
                    0x95, 0xFE, 0x04, 0xEF, 0x7A, 0x25, 0x76, 0x7F, 0x2E, 0x24};

char *expected =
    "84B0D432F050DABDD3E15D34BE84304231917C011AC2B713A0AF84810CB7D24B";

int test_hash() {
    bitstring_t bits = (bitstring_t){.length = 20 * 8, .data = data};

    char *output = malloc(65); // hash in hex is 64 + null byte
    bytes_to_hex(hash(bits), 32, output);

    int comparison = strncmp(output, expected, 40);
    free(output);

    if (comparison) {
        printf("test_hash:Hash does not produce correct output\nexpected: "
               "%s\noutput: %s\n",
               expected, output);
        return 0;
    }

    return 1;
}
