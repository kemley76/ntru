#include "arithmetic.h"
#include "../arithmetic.h"
#include "../constants.h"
#include "../encodings.h"
#include "../utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_all_arithmetic() {
    int status = 1;
    status &= test_poly_mul_1();

    if (status) {
        printf("All arithmeitc tests passed\n");
    }
    return status;
}

// Test with sage:
char *hex1 =
    "04BF6381230AE2AE348F464805328263264F5E791710B3E52345909597A0657707C9681DA0"
    "44BEC39E25B063D91FD6D6585AC74CE0A520A7440B407CCA7D0772945EE53A09E1ED6868B7"
    "BF4D912C1E91CD109578ACE0990F74D37F5D907DCB8F460EAD0F18BDE0A89420E643122565"
    "BD542D4A1A49531EDFB9DC793D8E21F06F228E339583D296C64EBD7FA1";

char *hex2 =
    "D9BB5C2CD48004D978B77DC402903962E006E01DEF27910CB52C276F4035D1419E003B2721"
    "81570033A279010196360C6F7494B66AE15982E2BC54594BCD39179B48A85796EAAD53214F"
    "E7443A154C642AE1F1A2B1906B5AEE429769C014018DDD42DBB6C143C9E17303160CB55C45"
    "09D733C38726E9742ACB3D3A5AEDB2AD211F8E81D0034D9ACD14704E0A";

char *expected_product =
    "4D8E97C825659221727D3A1B40F1C004580C8A36BFBA452C3362350AF29B57CFE33A8FACB1"
    "B9ACECA11186B18D4405C9E7A211480778846D951737B0828289404E8D99C7C5EC9C442807"
    "E585F15B426C49CF2A8ED5066F9DB6DED1DE04BAB3DD9BE6144CD8AE2417846736CAD68C14"
    "4CDE5A4BB79D168558AF85612CAB8E591FB4B438F01C83D66535C4E69C";

//
// p1 = unpack_S3([list(reversed(Integer(a).digits(2, padto=8))) for a in
// list(bytes.fromhex(hex1))])
// p2 = unpack_S3([list(reversed(Integer(a).digits(2, padto=8))) for
// 	  a in list(bytes.fromhex(hex2))])
int test_poly_mul_1() {
    uint8_t *bytes1 = malloc(PACKED_S3_BYTES);
    uint8_t *bytes2 = malloc(PACKED_S3_BYTES);
    hex_to_bytes(hex1, PACKED_S3_BYTES, bytes1);
    hex_to_bytes(hex2, PACKED_S3_BYTES, bytes2);

    poly *p1 = unpack_S3(bytes1);
    poly *p2 = unpack_S3(bytes2);
    poly *result = S3(poly_mul_S(p1, p2));

    uint8_t *packed = malloc(PACKED_S3_BYTES);
    pack_S3(result, packed);

    char *hex_output = malloc(PACKED_S3_BYTES * 2 + 1);
    bytes_to_hex(packed, PACKED_S3_BYTES, hex_output);

    if (strncmp(expected_product, hex_output, PACKED_S3_BYTES * 2)) {
        printf("test_poly_mul_1: product does not match\n%s\n!=%s\n",
               expected_product, hex_output);
        return 0;
    }

    printf("test_poly_mul_1: test passed\n");
    return 1;
}

void print_poly(const char *name, poly *a) {
    printf("%s: ", name);
    for (int i = N; i >= 0; i--) {
        if (a->coeffs[i]) {
            printf("%dx^%d + ", a->coeffs[i], i);
        }
    }
    printf("\n");
}
