
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
    "4C3E231A198D2865503BCEBDC7E4483E4417C39F0F5DCEC6FDD740CAC6540E214A2C9F7600"
    "0E821B82E7E56EEAAC771701EEFACC4EEA815F3286847AA7D9F7901B36FA1348172CD44EAD"
    "971C6CEE5B0C3CE7FFC135FB917881F8AABC9FC65DB571EE8DC72D52EBEC03A8F28941E32A"
    "93559EFC09E02DFD6E47F102727FEB4175E2453B4B8F613E7CC6BC4AB35C89A061D9C72810"
    "8294C776D66B38D390E219CB63E7E969EB43CCDF5D83FAAB4A6E0C2B771269090BF44E3FAF"
    "A4DD0F48F4CBA945437E709C54513CE92928862DABC47080A53EF55BC9707AF6E0A8D5A994"
    "4F3D54D937FC9471440B48CD8E1E72709E37222AD7147B7C258B2B54A950AA8EDDD440AA9A"
    "D6E6DB1AC05FA603654CB0339F56C3007125DA8C8AD59B9DCF45747481BC4E65012707EB52"
    "C343FD49902AA2F1A261AB9D61EFF15AB9A6F3173644D6C70CF4CBECC1A8461E4C04644147"
    "AB1095810C4B87040204AF1F08B7ED2EB7B1C6DE9C79B81C607E50C0AC09A9722E34ADF5E7"
    "FAD722DD4AB08D831AD7621CE86CC0F2314880307DC99BE49BD37B904B61229CC44F449686"
    "80B15C73A32C853F68ACA81E543CDD99A367003C3D3966A725B4BB510CCE37887C5EF919BB"
    "AA2100C502F8E07E49EE0C767179AC1CF0B83A67071A9868384D1297CA58D4F654518D9573"
    "51E6238DA2734EB77A837A938A9FEA27A0F33E8DF5B02E141C44C96B3B9694DB1CD2A9A758"
    "AC94289DEAB4E86F14209FC71C8D854475D4816CE3928F9B1F15F755CA55C6032897289AD2"
    "018870FD0078CA6BCE4BB5B45CFAE74E9F076108043A240C249301FCADFF3722FA5F38A4B0"
    "1AEFC8D470BF6BF8098533565F2728592A3D87AC77F40E7CF098DC70D069E36DA461404960"
    "61EE16B166AB953B96FE097870F42C0B0BCFC73EC5AFB5552511601AA47ADAEA056B1F1F47"
    "2B1B6A2ECCCACB32D6061548B2D53AE686D61BAF321EE364A9A745F53FACF05B019EC6B5F0"
    "4CEE71ADEB6A54021E2C0F667647AF03B9CB416A63C287D95087A397FDE92ED46282B7662A"
    "75525F404A2DC235D619C5C216644496D8D3CCAABFBF3EC193B0B78DB41918EF655D085016"
    "5357E6D456E6A73DA683064120308EF8D536B32EB72EF5B6C39FC553C0D79A9B5CF874B580"
    "EB6A9055BB30032EF412FF87A20DE4B7BDD83CB92E10A21B7B3A9A1D6CDC302A4FD02690F1"
    "6239FBA4D4F4A42E8BDF875AE08F52AD66ACB27D1B595394EA23BE98BDACBE232E87C405D0"
    "8CD084E4A2C2D74EC24B9C88B96B5CB8733FF7657B29D047B8C6EC2AA0A1BC20862A36EF7B"
    "24750DBC93778B654DAD2773234BBB8328982DE73660C0C09AB9586ABA0CB56DF70F65DFEA"
    "140659E28EF8F40069EC691A144AFFE9316983C07B09F8F1FC7A02DD93DAC0DAACE44855A9"
    "28614F90DBA061C3C79CF7A5CC018AF125C714E8706D7B3F72F607D8DB6D372BF24189757D"
    "9EE74A3A966D417291E8D9D4606C6F40D54156B8BD2BD671121B0EE0325BAB1B73B7A27F9E"
    "C3B5F6FD54F0969A327B73F4B06D8A176C301D6179CAB246E997938DA3FF0CD6B6154782D2"
    "FB148441B4D1DCB639F87C9251AE5A1F819F46CC7D8B2A6898EF7DE52E01C696EA658C27BC"
    "6DDF25932B679320CD09ED9579DD0EB11A5D2F308AF23A4EB13554AB66554BA968BBC92B9A"
    "C8839D99B247915F7D34ED03FEC4CF3E511FE0EDDB23C1C5792ABBD7AF9718F216D90E5BBD"
    "38BD34A521A7493E85546DB9BF88A9E39635815E1F2B5E6A34C43A7225AFC93B0DD348CBAD"
    "DFA9FF47440AF6158C258D07F22CAC15501145FA953FD638544919EA3B37D2FB0C9A778BF1"
    "84997653AD34A38312C09ACE7E7709110C83C64AC91482E9A2F74F2A431A72BD34BA071833"
    "40E08A99EEBC7133462964DEFA876E70623DED0F77520626EFBD55BD368F074E3ABFF6C178"
    "D1FD2802106F2EAC1BC94F69ED02C407DB304FEE143AEEE8B4612A5CFF883A";

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
    uint8_t *packed_r = malloc(PACKED_S3_BYTES);
    uint8_t *packed_m = malloc(PACKED_S3_BYTES);
    pack_S3(rm.first, packed_r);
    pack_S3(rm.second, packed_m);

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
