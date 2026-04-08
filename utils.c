#include <stdint.h>
#include <stdio.h>

// Need to have one byte extra for null terminator
void bytes_to_hex(uint8_t *bytes, int n_bytes, char *hex) {
    for (int i = n_bytes - 1; i >= 0; i--) {
        sprintf(hex + 2 * (n_bytes - 1 - i), "%02hhX", bytes[n_bytes - 1 - i]);
    }
}

void hex_to_bytes(char *hex, int n_bytes, uint8_t *bytes) {
    for (int i = 0; i < n_bytes; i++) {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}
