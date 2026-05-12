#ifndef KAT_H
#define KAT_H

#include "../constants.h"
#include "../kem.h"
#include <stdio.h>

// Parses the KAT file and checks and times our implementation against the tests
// contained in it
int test_all_kat();

// put comment here (a specific hex_seed)
int test_kat(uint8_t *seed, const char *pk, const char *sk, const char *ct,
             const char *ss);

#endif
