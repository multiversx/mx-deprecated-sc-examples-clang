#include <stdint.h>

#ifndef __SC_BYTES__

#define __SC_BYTES__
typedef uint8_t byte;

struct BytesLibrary {
  byte* (*from_storage)(byte[32], uint64_t, uint64_t);
  byte* (*from_argument)(uint8_t, uint64_t*);
};

extern const struct BytesLibrary Bytes;

#endif
