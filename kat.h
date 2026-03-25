#ifndef KAT_H
#define KAT_H

#include "constants.h"
#include "kem.h"
// need to include time + ctypes in C

// Need to reference a shared library or some equivalent
// aka: lib = ...

// Parses the KAT file and checks and times our implementation against the tests contained in it
void test_all();

// put comment here (a specific hex_seed)
void test_kat();

// put comment here
void bytes_to_hex();

// put comment here
void to_hex_str();

#endif