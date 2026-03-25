#include "kem.h"

// Generates a public and a private key for use in the key exchange
void Key_Pair(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Generates a shared secret key and a ciphertext. The ciphertext can be 
// used with the private key to recover the same shared secret key
void Encapsulate(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Recovers the shared secret key from the ciphertext using the known private key
// There is a very small chance of failure, in which Decapsulate will return a random key 
void Decapsulate(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}

// Hashes a bitstring using sha3_256
void hash(){
    fprintf(stderr, "Error: Function is not implemented.\n");
    exit(EXIT_FAILURE);
}