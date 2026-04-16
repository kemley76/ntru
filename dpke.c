#include "dpke.h"
#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include "encodings.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// input: coins (bit string of length sample_key_bits)
// output: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_public_key (byte array of length dpke_public_key_bytes)
DPKE_key_pair_t DPKE_Key_Pair(bitstring_t coins) {
    assert(coins.length == SAMPLE_KEY_BITS);
    poly_pair fg = Sample_fg(coins);
    poly *f = fg.first;
    poly *g = fg.second;
    poly *f_p = S3_inverse(f);

    // Note that this modifies g to scale by 3
    poly_pair pub_key = DPKE_Public_Key(f, g);
    poly *h = pub_key.first;
    poly *h_q = pub_key.second;

    uint8_t *packed_private_key = malloc(PACKED_S3_BYTES * 2 + PACKED_SQ_BYTES);
    pack_S3(f, packed_private_key);
    pack_S3(f_p, packed_private_key + PACKED_S3_BYTES);
    pack_Sq(h_q, packed_private_key + PACKED_S3_BYTES * 2);

    uint8_t *packed_public_key = malloc(PACKED_RQ0_BYTES);
    pack_Rq0(h, packed_public_key);

    return (DPKE_key_pair_t){.packed_public_key = packed_public_key,
                             .packed_private_key = packed_private_key};
}

// input: f (polynomial in lattice{f})
//       g (polynomial in lattice{g})
// output: h (polynomial that satisfies Rq(h · f) = 3 · g)
//           h_q (polynomial that satisfies Sq(h · hq) = 1)
poly_pair DPKE_Public_Key(poly *f, poly *g) {
    // G = g * 3
    poly *G = calloc(1, sizeof(poly));

    for (int i = 0; i < N; i++) {
        G->coeffs[i] = g->coeffs[i] * 3;
    }

    poly *v_0 = Sq(poly_mul_S(G, f));
    // print_poly("h", v_0);
    poly *v_1 = Sq_inverse(v_0);
    poly *temp = Rq(poly_mul_Rq(v_1, G));
    poly *h = Rq(poly_mul_Rq(temp, G));
    free(temp);

    temp = Rq(poly_mul_Rq(v_1, f));
    poly *h_q = Rq(poly_mul_Rq(temp, f));

    return (poly_pair){.first = h, .second = h_q};
}

// input: packed_public_key (byte array of length dpke_public_key_bytes)
//           packed_rm (byte array of length dpke_plaintext_bytes)
// output: packed_ciphertext (byte array of length dpke_ciphertext_bytes)
uint8_t *DPKE_Encrypt(uint8_t *packed_public_key, uint8_t *packed_rm) {
    uint8_t *packed_r = packed_rm;
    uint8_t *packed_m = packed_rm + PACKED_S3_BYTES;

    poly *r = S3_bar(unpack_S3(packed_r));
    poly *m_0 = unpack_S3(packed_m);
    poly *m_1 = Lift(m_0);
    poly *h = unpack_Rq0(packed_public_key);
    poly *rh_temp = Rq(poly_mul_Rq(r, h));

    for (int i = 0; i < N; i++) {
        rh_temp->coeffs[i] += m_1->coeffs[i];
    }
    poly *cipher = Rq(rh_temp);

    uint8_t *packed_ciphertext = malloc(PACKED_RQ0_BYTES);
    pack_Rq0(cipher, packed_ciphertext);

    return packed_ciphertext;
}

// input: packed_private_key (byte array of length dpke_private_key_bytes)
//           packed_ciphertext (byte array of length dpke_ciphertext_bytes)
// output: packed_rm (byte array of length dpke_plaintext_bytes)
//           fail (bit)
uint8_t *DPKE_Decrypt(uint8_t *packed_private_key, uint8_t *packed_ciphertext) {
    uint8_t *packed_f = packed_private_key;
    uint8_t *packed_fp = packed_private_key + PACKED_S3_BYTES;
    uint8_t *packed_hq = packed_private_key + PACKED_S3_BYTES * 2;

    poly *cipher = unpack_Rq0(packed_ciphertext);
    poly *f = S3_bar(unpack_S3(packed_f));
    poly *f_p = unpack_S3(packed_fp);

    poly *h_q = unpack_Sq(packed_hq);
    poly *v_1 = Rq_bar(poly_mul_S(cipher, f));
    poly *m_0 = S3_bar(poly_mul_S(v_1, f_p));
    poly *m_1 = Lift(m_0);

    // cipher - m_1
    for (int i = 0; i < N; i++) {
        cipher->coeffs[i] -= m_1->coeffs[i];
    }
    poly *r = Sq_bar(poly_mul_S(cipher, h_q));

    uint8_t *packed_rm = malloc(PACKED_S3_BYTES * 2);
    pack_S3(r, packed_rm);
    pack_S3(m_0, packed_rm + PACKED_S3_BYTES);

    // TODO TODO TODO
    // Check if there is a decryption failure. Is possible but very very rare

    return packed_rm;
}
