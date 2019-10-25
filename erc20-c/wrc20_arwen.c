

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
void getOwner(i32ptr* resultOffset);
void loadBalance(i32ptr* addressOffset, int32_t result);
int32_t blockHash(long long nonce, i32ptr* resultOffset);
int32_t loadArgumentAsBytes(int32_t id, i32ptr* argOffset);
void loadArgumentAsBig(int32_t id, int32_t destination);
long long getArgumentAsInt64(int32_t id);
int32_t getFunction(i32ptr* functionOffset);
int32_t getNumArguments();
int32_t storageStore(i32ptr* keyOffset, i32ptr* dataOffset, int32_t dataLength);
int32_t storageLoad(i32ptr* keyOffset, i32ptr* dataOffset);
int32_t storageStoreAsBigInt(i32ptr* keyOffset, int32_t source);
int32_t storageLoadAsBigInt(i32ptr* keyOffset, int32_t destination);
int32_t storageStoreAsInt64(i32ptr* keyOffset, long long value);
long long storageLoadAsInt64(i32ptr* keyOffset);
void getCaller(i32ptr* resultOffset);
int32_t getCallValue(i32ptr* resultOffset);
long long getCallValueAsInt64();
void logMessage(int32_t pointer, int32_t length);
void writeLog(int32_t pointer, int32_t length, int32_t topicPtr, int32_t numTopics);
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
bytes32 sender[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bytes32 recipient[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void init() {
  if (getNumArguments() != 1) {
    signalError();
    return;
  }

  getCaller((i32ptr*)sender);
  int32_t totalAmount = bigInsert(0);
  loadArgumentAsBig(totalAmount, 0);

  storageStoreAsBigInt((i32ptr*)sender, totalAmount);
}

void do_balance() {
  if (getNumArguments() != 0) {
    signalError();
    return;
  }

  int32_t balance = bigInsert(0);
  storageLoadAsBigInt((i32ptr*)sender, balance);

  returnBigInt(balance);
}

void transfer() {
  if (getNumArguments() != 2) {
    signalError();
    return;
  }

  getCaller((i32ptr*)sender);
  loadArgumentAsBytes(0, (i32ptr*)recipient);
  int32_t amount = bigInsert(0);
  loadArgumentAsBig(amount, 1);

  int32_t senderBalance = bigInsert(0);
  storageLoadAsBigInt((i32ptr*)sender, senderBalance);

  if (bigCmp(amount, senderBalance) > 0) {
    signalError();
    return;
  }

  bigSub(senderBalance, senderBalance, amount);
  storageStoreAsBigInt((i32ptr*)sender, senderBalance);

  int32_t receiverBalance = bigInsert(0);
  storageLoadAsBigInt((i32ptr*)recipient, receiverBalance);
  bigAdd(receiverBalance, receiverBalance, amount);
  storageStoreAsBigInt((i32ptr*)recipient, receiverBalance);
}

void topUp() {
}

// global data used in next function, will be allocated to WebAssembly memory
i32 selector[1] = {0};
void _main(void) {
}
