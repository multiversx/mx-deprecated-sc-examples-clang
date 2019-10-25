#include "sc_bytes.h"
#include "sc_bigints.h"

int main(void) {
  byte myKey[32] = {0, 0, 0, 0, 0, 0, 0, 0, 
                    0, 0, 0, 0, 0, 0, 0, 0, 
                    0, 0, 0, 0, 0, 0, 0, 0, 
                    0, 0, 0, 0, 0, 0, 0, 0}; 

  // Load some bytes from storage (the -1 values mean that the entire contents
  // stored under the given key are loaded; alternatively, only a part of the
  // contents can also be loaded, specifying offset and length).
  byte *stored_value = Bytes.from_storage(myKey, -1, -1);

  uint64_t argument_size = 0;
  byte *argument_value = Bytes.from_argument(2, &argument_size);

  bigint mybigint = BigInts.from_argument(3, &argument_size);
  bigint myzero = BigInts.zero();
  bigint myone = BigInts.one();

  BigInts.add(mybigint, myone);
  if (BigInts.isZero(mybigint)) {
    // do something
  }

  if (BigInts.isOne(mybigint)) {
    // do something else
  }

  return 0;
}
