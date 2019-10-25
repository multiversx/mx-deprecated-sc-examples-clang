#include <stddef.h>
#include <stdbool.h>
#include "sc_bigints.h"

bigint BigInts_from_storage(byte key[32], uint64_t offset, uint64_t length) {
  // Similar to Bytes.from_storage(), just that the loaded bytes are converted
  // to a bigint.
  
  return 0;
} 

bigint BigInts_from_argument(uint8_t arg_index, uint64_t* length_out) {
  // Similar to Bytes.from_argument(), just that the argument bytes are converted
  // to a bigint.
  
  return 0;
}

bigint BigInts_zero() {
  return 0;
}

bigint BigInts_one() {
  return 0;
}

bool BigInts_isZero(bigint num) {
  return true;
}

bool BigInts_isOne(bigint num) {
  return true;
}

void BigInts_add(bigint a, bigint b) {
}

const struct BigIntsLibrary BigInts = {
  .from_storage = BigInts_from_storage,
  .from_argument = BigInts_from_argument,
  .zero = BigInts_zero,
  .one = BigInts_one,
  .add = BigInts_add,
  .isZero = BigInts_isZero,
  .isOne = BigInts_isOne
};

