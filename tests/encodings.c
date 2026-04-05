
#include "../encodings.h"
#include "../utils.h"
#include "encodings.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *bytes_hex = "AB23893A89003298AB091230190009123ABDE2231"; // 40 hex digits
// return 1 for pass, 0 for fail
int test_all_encodings() {
    int status = 1;
    status &= test_byte_encodings_1();
    status &= test_byte_encodings_2();

    if (status) {
        printf("All encoding tests passed\n");
    }
    return status;
}

int test_byte_encodings_1() {
    // hex -> bytes -> bits -> bytes -> hex
    // ensures that the input and output are the same
    uint8_t raw_bytes[20];
    hex_to_bytes(bytes_hex, 20, raw_bytes);

    bitstring_t bits = bytes_to_bits(raw_bytes, 20 * 8);
    uint8_t result_bytes[20];
    bits_to_bytes(bits, result_bytes);

    char result_hex[41];
    bytes_to_hex(result_bytes, 20, result_hex);

    if (strncmp(bytes_hex, result_hex, 40)) {
        printf("test_byte_encodings_1: output %s != expected %s\n", result_hex,
               bytes_hex);
        return 0;
    }
    printf("test_byte_encodings_1: test passed\n");
    return 1;
}

// 9 * 8 + 5 = 77 bits (the 3 at the end are not part of the bitstring
uint8_t test2[10] = {0x1A, 0x2C, 0x4A, 0xFA, 0x33,
                     0xAB, 0x39, 0x49, 0x38, 0b00010101};

int test_byte_encodings_2() {
    // bits -> bytes -> bits
    // ensures that the input and output are the same
    bitstring_t bits = new_bistring(77); // use weird number to test edge cases
    memcpy(bits.data, test2, 10);        // load test2 bitstring into bits

    uint8_t raw_bytes[10];
    bits_to_bytes(bits, raw_bytes);
    bitstring_t result_bits = bytes_to_bits(raw_bytes, 77);

    for (int i = 0; i < 10; i++) {
        if (bits.data[i] != result_bits.data[i]) {
            printf("test_byte_encodings_2: output %X != expected %X\n",
                   result_bits.data[i], bits.data[i]);
            return 0;
        }
    }

    printf("test_byte_encodings_2: test passed\n");
    return 1;
}
