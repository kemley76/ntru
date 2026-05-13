#include "dpke.h"
#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include "encodings.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// input: coins (bit string of length sample_key_bits)
// output: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_public_key (byte array of length dpke_public_key_bytes)
void DPKE_Key_Pair(bitstring_t coins, uint8_t *packed_private_key,
                   uint8_t *packed_public_key) {
    assert(coins.length == SAMPLE_KEY_BITS);
    poly f = {0}, g = {0}, f_p = {0}, h = {0}, h_q = {0};
    printf("sample fg\n");
    Sample_fg(coins, &f, &g);
    printf("inversing..\n");
    S3_inverse(&f, &f_p);

    // Note that this modifies g to scale by 3
    printf("public key..\n");
    DPKE_Public_Key(&f, &g, &h, &h_q);

    pack_S3(&f, packed_private_key);
    pack_S3(&f_p, packed_private_key + PACKED_S3_BYTES);
    pack_Sq(&h_q, packed_private_key + PACKED_S3_BYTES * 2);
    pack_Rq0(&h, packed_public_key);
}

// input: f (polynomial in lattice{f})
//       g (polynomial in lattice{g})
// output: h (polynomial that satisfies Rq(h · f) = 3 · g)
//           h_q (polynomial that satisfies Sq(h · hq) = 1)
void DPKE_Public_Key(poly *f, poly *g, poly *h, poly *h_q) {
    // G = g * 3
    poly G = {0};

    for (int i = 0; i < N; i++) {
        G.coeffs[i] = g->coeffs[i] * 3;
    }

    poly v_0 = {0};
    poly_mul_S(&G, f, &v_0);
    Sq(&v_0);

    // print_poly("h", v_0);
    poly v_1 = {0}, temp = {0};
    printf("sq inverse\n");
    Sq_inverse(&v_0, &v_1);

    poly_mul_Rq(&v_1, &G, &temp);
    Rq(&temp);

    poly_mul_Rq(&temp, &G, h);
    Rq(h);

    poly_mul_Rq(&v_1, f, &temp);
    Rq(&temp);

    poly_mul_Rq(&temp, f, h_q);
    Rq(h_q);
}

// input: packed_public_key (byte array of length dpke_public_key_bytes)
//           packed_rm (byte array of length dpke_plaintext_bytes)
// output: packed_ciphertext (byte array of length dpke_ciphertext_bytes)
void DPKE_Encrypt(uint8_t *packed_public_key, uint8_t *packed_rm,
                  uint8_t *packed_ciphertext) {
    uint8_t *packed_r = packed_rm;
    uint8_t *packed_m = packed_rm + PACKED_S3_BYTES;

    poly r = {0}, m_0 = {0}, m_1 = {0}, h = {0}, cipher = {0};
    unpack_S3(packed_r, &r);
    S3_bar(&r);

    unpack_S3(packed_m, &m_0);
    Lift(&m_0, &m_1);
    unpack_Rq0(packed_public_key, &h);

    poly_mul_Rq(&r, &h, &cipher);
    Rq(&cipher);

    for (int i = 0; i < N; i++) {
        cipher.coeffs[i] += m_1.coeffs[i];
    }
    Rq(&cipher);

    pack_Rq0(&cipher, packed_ciphertext);
}

// input: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_ciphertext (byte array of length dpke_ciphertext_bytes)
// output: packed_rm (byte array of length dpke_plaintext_bytes)
//           fail (bit)
void DPKE_Decrypt(uint8_t *packed_private_key, uint8_t *packed_ciphertext,
                  uint8_t *packed_rm) {
    uint8_t *packed_f = packed_private_key;
    uint8_t *packed_fp = packed_private_key + PACKED_S3_BYTES;
    uint8_t *packed_hq = packed_private_key + PACKED_S3_BYTES * 2;

    poly cipher = {0}, f = {0}, f_p = {0}, h_q = {0}, v_1 = {0}, m_0 = {0},
         m_1 = {0},
         r = {0}; // TODO: Is there a way to not use so much stack space??
    unpack_Rq0(packed_ciphertext, &cipher);

    unpack_S3(packed_f, &f);
    S3_bar(&f);

    unpack_S3(packed_fp, &f_p);

    unpack_Sq(packed_hq, &h_q);
    poly_mul_S(&cipher, &f, &v_1);
    Rq_bar(&v_1);

    poly_mul_S(&v_1, &f_p, &m_0);
    S3_bar(&m_0);
    Lift(&m_0, &m_1);

    // cipher - m_1
    for (int i = 0; i < N; i++) {
        cipher.coeffs[i] -= m_1.coeffs[i];
    }
    poly_mul_S(&cipher, &h_q, &r);
    Sq_bar(&r);

    pack_S3(&r, packed_rm);
    pack_S3(&m_0, packed_rm + PACKED_S3_BYTES);

    // TODO TODO TODO
    // Check if there is a decryption failure. Is possible but very very rare
}
