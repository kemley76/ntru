#ifndef KAT_H
#define KAT_H

#include "constants.h"
#include "kem.h"
#include <stdio.h>

// Parses the KAT file and checks and times our implementation against the tests
// contained in it
void test_all();

// put comment here (a specific hex_seed)
void test_kat();

#endif
