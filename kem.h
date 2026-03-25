#ifndef KEM_H
#define KEM_H

#include "constants.h"
#include "dpke.h"
#include "encodings.h"
#include "sampling.h"
// need to include random, hashlib, and ceil from math but in C

// Generates a public and a private key for use in the key exchange
void Key_Pair();

// Generates a shared secret key and a ciphertext. The ciphertext can be 
// used with the private key to recover the same shared secret key
void Encapsulate();

// Recovers the shared secret key from the ciphertext using the known private key
// There is a very small chance of failure, in which Decapsulate will return a random key 
void Decapsulate();

// Hashes a bitstring using sha3_256
void hash();

#endif