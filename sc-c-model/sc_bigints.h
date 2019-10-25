#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "sc_bytes.h"

#ifndef __SC_BIGINTS__

#define __SC_BIGINTS__

typedef uint8_t bigint;

struct BigIntsLibrary {
  bigint (*from_storage)(byte[32], uint64_t, uint64_t);
  bigint (*from_argument)(uint8_t, uint64_t*);
  bigint (*zero)();
  bigint (*one)();
  bool   (*isZero)(bigint);
  bool   (*isOne)(bigint);
  void   (*add)(bigint, bigint);
  // other operations here
};

extern const struct BigIntsLibrary BigInts;

#endif
