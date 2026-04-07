// testing file, like we had in sage

#include "./tests/encodings.h"
#include "./tests/utils.h"
#include "kat.h"
#include <stdint.h>

int main(int argc, char **argv) {
    test_all_encodings();
    test_all_utils();
}
