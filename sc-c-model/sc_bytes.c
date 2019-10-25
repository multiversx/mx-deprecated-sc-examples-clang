#include <stddef.h>
#include "sc_bytes.h"

byte* from_storage(byte key[32], uint64_t offset, uint64_t length) {
  // If offset is -1, load from storage the all the bytes found under the key.
  // If offset is > -1, but length == -1, load all the bytes starting from offset until the end.
  // If both offset and length are valid, then load only the requested bytes from storage.
  
  return NULL;
} 

byte* from_argument(uint8_t arg_index, uint64_t* length_out) {
  // Return the bytes read from argument number arg_index, and write size of
  // the argument's value at length_out.
  
  return NULL;
}

const struct BytesLibrary Bytes = {
  .from_storage = from_storage,
  .from_argument = from_argument
};
