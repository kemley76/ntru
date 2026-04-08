
#include "../encodings.h"
#include "../sampling.h"
#include "../utils.h"
#include "encodings.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *bytes_hex = "AB23893A89003298AB091230190009123ABDE2231"; // 40 hex digits
// return 1 for pass, 0 for fail
int test_all_encodings() {
    int status = 1;
    status &= test_byte_encodings_1();
    status &= test_byte_encodings_2();
    status &= test_pack_unpack_S3();

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
    memcpy(bits.data, test2, sizeof test2); // load test2 bitstring into bits

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

char *sample_bits =
    "327CC45898B114A60ADC73BDE327127C22E8C3F9F0BA7363BFEB0253632A70845234F96E00"
    "7041D841E7A7765735EEE880775F33725781FA4C61215EE59BEF09D86C5FC812E8342B72B5"
    "E9383677DA303CE7FF83ACDF891E811F553DF963BAAD8E77B1E3B44AD737C0154F9182C754"
    "C9AA793F9007B4BF76E28F404EFED782AE47A2DCD2F1867C3E633D52CD3A9105869BE31408"
    "4129E36E6BD61CCB094798D3C6E79796D7C233FBBAC15FD5527630D4EE489690D02F72FCF5"
    "25BBF0122FD395A2C27E0E392A8A3C97941461B4D5230E01A57CAFDA930E5E6F0715AB9529"
    "F2BC2A9BEC3F298E22D012B371784E0E79EC4454EB28DE3EA4D1D42A950A5571BB2B025559"
    "6B67DB5803FA65C0A6320DCCF96AC3008EA45B3151ABD9B9F3A22E2E813D72A680E4E0D74A"
    "C3C2BF920954458F4586D5B986F78F5A9D65CFE86C226BE3302FD3378315627832202682E2"
    "D508A98130D2E1204020F5F810EDB774ED8D637B399E1D38067E0A033590954E742CB5AFE7"
    "5FEB44BB520DB1C158EB46381736034F8C12010CBE93D927D9CBDE09D286443923F2226961"
    "018D3ACEC534A1FC163515782A3CBB99C5E6003CBC9C66E5A42DDD8A3073EC113E7A9F98DD"
    "558400A3401F077E9277306E8E9E35380F1D5CE6E05819161CB248E9531A2B6F2A8AB1A9CE"
    "8A67C4B145CE72ED5EC15EC951F957E405CF7CB1AF0D7428382293D6DC6929DB384B95E51A"
    "352914B9572D17F62804F9E338B1A122AE2B8136C749F1D9F8A8EFAA53AA63C014E914594B"
    "80110EBF001E53D673D2AD2D3A5FE772F9E08610205C243024C9803FB5FFEC445FFA1C250D"
    "58F7132B0EFDD61F90A1CC6AFAE4149A54BCE135EE2F703E0F193B0E0B96C7B62586029206"
    "8677688D66D5A9DC697F901E0E2F34D0D0F3E37CA3F5ADAAA4880658255E5B57A0D6F8F8E2"
    "D4D856743353D34C6B60A8124DAB5C67616BD8F54C78C72695E5A2AFFC350FDA807963AD0F"
    "32778EB5D7562A407834F0666EE2F5C09DD38256C643E19B0AE1C5E9BF97742B4641ED6654"
    "AE4AFA0252B443AC6B98A343682622691BCB3355FDFD7C83C90DEDB12D9818F7A6BA100A68"
    "CAEA672B6A67E5BC65C16082040C711FAB6CCD74ED74AF6DC3F9A3CA03EB59D93A1F2EAD01"
    "D75609AADD0CC0742F48FFE145B027EDBD1B3C9D740845D8DE5C59B8363B0C54F20B64098F"
    "469CDF252B2F2574D1FBE15A07F14AB566354DBED89ACA2957C47D19BD357DC474E123A00B"
    "310B21274543EB7243D239119DD63A1DCEFCEFA6DE940BE21D63375405853D0461546CF7DE"
    "24AEB03DC9EED1A6B2B5E4CEC4D2DDC11419B4E76C060303599D1A565D30ADB6EFF0A6FB57"
    "28609A47711F2F00963796582852FF978C96C103DE901F8F3F5E40BBC95B035B352712AA95"
    "1486F209DB0586C3E339EFA53380518FA4E328170EB6DEFC4E6FE01BDBB6ECD44F8291AEBE"
    "79E7525C69B6824E89179B2B0636F602AB826A1DBDD46B8E48D870074CDAD5D8CEED45FE79"
    "C3AD6FBF2A0F69594CDECE2F0DB651E8360CB8869E534D6297E9C9B1C5FF306B6DA8E2414B"
    "DF2821822D8B3B6D9C1F3E498A755AF881F96233BED1541619F7BEA77480636957A631E43D"
    "B6FBA4C9D4E6C904B390B7A99EBB708D58BAF40C514F5C728DAC2AD566AAD29516DD93D459"
    "13C1B9994DE289FABE2CB7C07F23F37C8AF807B7DBC483A39E54DDEBF5E9184F689B70DABD"
    "1CBD2CA584E5927CA12AB69DFD1195C769AC817AF8D47A562C235C4EA4F593DCB0CB12D3B5"
    "FB95FFE222506FA831A4B1E04F3435A80A88A25FA9FC6B1C2A929857DCEC4BDF3059EED18F"
    "21996ECAB52CC5C1480359737EEE908830C163529328419745EFF254C2584EBD2C5DE018CC"
    "02075199773D8ECC6294267B5FE1760E46BCB7F0EE4A6064F7BDAABD6CF1E0725CFD6F831E"
    "8BBF144008F67435D893F296B74023E0DB0CF277285C77172D86543AFF115C";

char *packed_output =
    "CB7B130DAB1925412FE6025E1969C14A875804B933782D8AB23B9177EEF2869BAB652605A9"
    "A63D023133B80A508FDCCF265095E85B92092B151FDFEC369B41E2CCDEB001EF37A2593C2C"
    "837619225BA49B5D4707DB123DED9B7549EFF0799F512657046393713CEFAAAE2F6DB81A50"
    "502A4199082186797C72B196E392BC081F847FA178E4DFCC3644C2209BDE4C0A6867B19807"
    "96E49D6343A35D61CA710F8E6D81D34A1812BDA43B3371E86EDDC73047D46CA5E560687B1C"
    "2D7CB110A2196B5C476F405277C0EAAAB6B4E902DA29C1B111729B7538B106DD22ED47A840"
    "1F61053873EE6CED69BF1FBE098D9B8E44660F464DD639D2CC20F2BCE3E442B7672EDD66E7"
    "7E11205B308FB42D2AB9D23F3AB76447AAE199BCDB";

int test_pack_unpack_S3() {
    uint8_t *bytes = malloc(SAMPLE_PLAINTEXT_BITS / 8);
    hex_to_bytes(sample_bits, SAMPLE_PLAINTEXT_BITS / 8, bytes);
    bitstring_t bits = bytes_to_bits(bytes, SAMPLE_PLAINTEXT_BITS);
    poly_pair rm = Sample_rm(bits);
    uint8_t *packed_r = pack_S3(rm.first);
    uint8_t *packed_m = pack_S3(rm.second);

    char *hex_output = malloc(PACKED_S3_BYTES * 4 + 1);
    bytes_to_hex(packed_r, PACKED_S3_BYTES, hex_output);
    bytes_to_hex(packed_m, PACKED_S3_BYTES, hex_output + PACKED_S3_BYTES * 2);

    if (strncmp(hex_output, packed_output, PACKED_S3_BYTES * 2)) {
        printf("test_pack_unpack_S3: output != expected\n");
        printf("output:\n%s\n\n", hex_output);
        printf("expected:\n%s\n\n", packed_output);
        return 0;
    }

    poly *unpacked_r = unpack_S3(packed_r);
    for (int i = 0; i < N - 1; i++) {
        if (unpacked_r->coeffs[i] != (rm.first->coeffs[i] + 9) % 3) {
            printf(
                "test_pack_unpack_S3: unpacking r failed at coefficient %d\n",
                i);
            return 0;
        }
    }
    free(unpacked_r);

    poly *unpacked_m = unpack_S3(packed_m);
    for (int i = 0; i < N - 1; i++) {
        if (unpacked_m->coeffs[i] != (rm.second->coeffs[i] + 9) % 3) {
            printf(
                "test_pack_unpack_S3: unpacking m failed at coefficient %d\n",
                i);
            return 0;
        }
    }
    free(unpacked_m);

    free(hex_output);
    printf("test_pack_unpack_S3: test passed\n");
    return 1;
}
