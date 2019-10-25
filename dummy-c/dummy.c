

// to avoid includes from libc, just hard-code things
typedef unsigned char uint8_t;
typedef int int32_t;
typedef int uint32_t;
typedef unsigned long long uint64_t;

// types used for Ethereum stuff
typedef uint8_t* bytes; // an array of bytes with unrestricted length
typedef uint8_t bytes32[32]; // an array of 32 bytes
typedef uint8_t address[32]; // an array of 32 bytes
typedef unsigned __int128 u128; // a 128 bit number, represented as a 16 bytes long little endian unsigned integer in memory
//typedef uint256_t u256; // a 256 bit number, represented as a 32 bytes long little endian unsigned integer in memory
typedef uint32_t i32; // same as i32 in WebAssembly
typedef uint32_t i32ptr; // same as i32 in WebAssembly, but treated as a pointer to a WebAssembly memory offset
typedef uint64_t i64; // same as i64 in WebAssembly

// functions for ethereum stuff
void getOwner(int32_t resultOffset);
void loadBalance(int32_t addressOffset, int32_t result);
int32_t blockHash(long long nonce, int32_t resultOffset);
int32_t loadArgumentAsBytes(int32_t id, int32_t argOffset);
void loadArgumentAsBig(int32_t id, int32_t destination);
long long getArgumentAsInt64(int32_t id);
int32_t getFunction(int32_t functionOffset);
int32_t getNumArguments();
int32_t storageStore(int32_t keyOffset, int32_t dataOffset, int32_t dataLength);
int32_t storageLoad(int32_t keyOffset, int32_t dataOffset);
int32_t storageStoreAsBigInt(int32_t keyOffset, int32_t source);
int32_t storageLoadAsBigInt(int32_t keyOffset, int32_t destination);
int32_t storageStoreAsInt64(int32_t keyOffset, long long value);
long long storageLoadAsInt64(int32_t keyOffset);
void getCaller(int32_t resultOffset);
int32_t getCallValue(int32_t resultOffset);
long long getCallValueAsInt64();
void logMessage(int32_t pointer, int32_t length);
void writeLog(int32_t pointer, int32_t length, int32_t topicPtr, int32_t numTopics);
void finish(int32_t dataOffset, int32_t length);
long long getBlockTimestamp();
void signalError();
int32_t bigInsert(int32_t smallValue);
int32_t bigByteLength(int32_t reference);
int32_t bigGetBytes(int32_t reference);
void bigSetBytes(int32_t destination, int32_t byteOffset, int32_t byteLength);
void bigAdd(int32_t destination, int32_t op1, int32_t op2);
void bigSub(int32_t destination, int32_t op1, int32_t op2);
void bigMul(int32_t destination, int32_t op1, int32_t op2);
int32_t bigCmp(int32_t op1, int32_t op2);
void returnBigInt(int32_t reference);
void returnInt32(int32_t value);
void debugPrintBig(int32_t value);
void debugPrintInt32(int32_t value);



// global data used in next function, will be allocated to WebAssembly memory
bytes32 addr[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

bytes32 some_bytes[1] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void init() {
  getOwner((i32ptr*)addr);
  int bi = bigInsert(257);
  storageStoreAsBigInt((i32ptr*)addr, bi);

  bigSetBytes(bi, (i32ptr*)some_bytes, (int32_t)7);
  debugPrintBig(bi);

  bigSub(bi, bi, bigInsert(2));
  debugPrintBig(bi);
  int32_t bi_length = bigByteLength(bi);
  debugPrintInt32(bi_length);

  int32_t newMem = bigGetBytes(bi);
  debugPrintInt32(newMem);
  newMem = bigGetBytes(bi);
  debugPrintInt32(newMem);

  bigSetBytes(bi, newMem-6, (int32_t)6);
  debugPrintBig(bi);

  bigMul(bi, bi, bigInsert(100));
  returnBigInt(bi);
}

void topUp() {
  int bi = bigInsert(17);
  loadArgumentAsBig(0, bi);
  debugPrintBig(bi);
  loadArgumentAsBig(1, bi);
  debugPrintBig(bi);
  loadArgumentAsBig(2, bi);
  debugPrintBig(bi);
  getOwner((i32ptr*)addr);
  loadBalance((i32ptr*)addr, bi);
  storageLoadAsBigInt((i32ptr*)addr, bi);
  debugPrintBig(bi);

  returnInt32(73);
}

void do_balance() {

}

void transfer() {

}

// global data used in next function, will be allocated to WebAssembly memory
i32 selector[1] = {0};
void _main(void) {
}
