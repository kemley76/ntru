#include "bitstrings.h"
#include <stdint.h>

void bytes_to_hex(uint8_t *bytes, int n_bytes, char *hex);
void hex_to_bytes(char *hex, int n_bytes, uint8_t *bytes);
uint8_t *hash(bitstring_t bits);
