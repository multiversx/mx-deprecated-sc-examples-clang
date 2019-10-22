
// to avoid includes from libc, just hard-code things
typedef unsigned char uint8_t;
typedef int int32_t;
typedef int uint32_t;
typedef unsigned long long uint64_t;

// types used for Ethereum stuff
typedef uint8_t* bytes; // an array of bytes with unrestricted length
typedef uint8_t bytes32[32]; // an array of 32 bytes
typedef uint8_t address[32]; // an array of 20 bytes
typedef unsigned __int128 u128; // a 128 bit number, represented as a 16 bytes long little endian unsigned integer in memory
//typedef uint256_t u256; // a 256 bit number, represented as a 32 bytes long little endian unsigned integer in memory
typedef uint32_t i32; // same as i32 in WebAssembly
typedef uint32_t i32ptr; // same as i32 in WebAssembly, but treated as a pointer to a WebAssembly memory offset
typedef uint64_t i64; // same as i64 in WebAssembly

// functions for ethereum stuff
void getCaller(i32ptr* resultOffset);
int32_t getArgument(i32 id, i32ptr* argOffset);
int32_t getNumArguments();
void signalError();
void finish(i32ptr* dataOffset, i32 dataLength);
i32 storageStore(i32ptr* pathOffset, i32ptr* resultOffset, i32 dataLength);
i32 storageLoad(i32ptr* pathOffset, i32ptr* resultOffset);
long long storageLoadAsInt64(i32ptr* keyOffset);
i32 storageStoreAsInt64(i32ptr* keyOffset, long long value);
i32 getCallValue(i32ptr* resultOffset);
uint64_t getArgumentAsInt64(i32 id);
   //the memory offset to load the path from (bytes32), the memory offset to store/load the result at (bytes32)

