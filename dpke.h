#ifndef DPKE_H
#define DPKE_H

#include "arithmetic.h"
#include "bitstrings.h"
#include "constants.h"
#include "encodings.h"
#include "sampling.h"
#include <stdint.h>

typedef struct {
    uint8_t *packed_private_key;
    uint8_t *packed_public_key;
} DPKE_key_pair_t;

// put comment here
DPKE_key_pair_t DPKE_Key_Pair();

// put comment here
void DPKE_Public_Key();

// put comment here
uint8_t *DPKE_Encrypt();

// put comment here
void DPKE_Decrypt();

#endif
