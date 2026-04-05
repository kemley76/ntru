#include "../utils.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *input_hex =
    "0615502300158C5EC95595FE04EF7A25767F2E24"; // 40 hex digits long
uint8_t expected_output[20] = {0x24, 0x2E, 0x7F, 0x76, 0x25, 0x7A, 0xEF,
                               0x04, 0xFE, 0x95, 0x55, 0xC9, 0x5E, 0x8C,
                               0x15, 0x00, 0x23, 0x50, 0x15, 0x06};

// return 1 for pass, 0 for fail
int test_all_utils() {
    // test the hex_to_bytes and bytes_to_hex
    uint8_t output_bytes[20];
    hex_to_bytes(input_hex, 20, output_bytes); // works

    // ensure the bytes match the expected output
    for (int i = 0; i < 20; i++) {
        if (output_bytes[i] != expected_output[i]) {
            printf("output %X != expected %X after hex_to_bytes\n",
                   output_bytes[i], expected_output[i]);
            return 0;
        }
    }

    char output_hex[41];
    bytes_to_hex(output_bytes, 20, output_hex);
    if (strncmp(input_hex, output_hex, 40)) { // strings don't match
        printf("bytes_to_hex: output %s != expected %s\n", output_hex,
               input_hex);
        return 0;
    }
    printf("bytes_to_hex/hex_to_bytes test passed\n");
    printf("All util tests passed\n");
    return 1;
}
