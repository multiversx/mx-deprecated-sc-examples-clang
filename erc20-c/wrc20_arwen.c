

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


i64 reverse_bytes(i64 a){
  i64 b = 0;
  b += (a & 0xff00000000000000)>>56;
  b += (a & 0x00ff000000000000)>>40;
  b += (a & 0x0000ff0000000000)>>24;
  b += (a & 0x000000ff00000000)>>8;
  b += (a & 0x00000000ff000000)<<8;
  b += (a & 0x0000000000ff0000)<<24;
  b += (a & 0x000000000000ff00)<<40;
  b += (a & 0x00000000000000ff)<<56;
  return b;
}


// global data used in next function, will be allocated to WebAssembly memory
bytes32 addy[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bytes32 balance[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void do_balance() {
  if (getNumArguments() < 1)
    signalError();

  // get address to check balance of, note: padded to 32 bytes since used as key
  getArgument(0, (i32ptr*)addy);

  // get balance
  storageLoad((i32ptr*)addy, (i32ptr*)balance);
  // return balance
  finish((i32ptr*)balance, 8);
}

// global data used in next function, will be allocated to WebAssembly memory
bytes32 sender[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bytes32 recipient[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bytes32 value[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bytes32 recipient_balance[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bytes32 sender_balance[1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void transfer() {
  if (getNumArguments() < 2)
    signalError();

  // get caller
  getCaller((i32ptr*)sender);

  getArgument(0, (i32ptr*)recipient);
  getArgument(1, (i32ptr*)value);

  // get balances
  storageLoad((i32ptr*)sender, (i32ptr*)sender_balance);
  storageLoad((i32ptr*)recipient, (i32ptr*)recipient_balance);
  *(i64*)sender_balance = reverse_bytes(*(i64*)sender_balance);
  *(i64*)recipient_balance = reverse_bytes(*(i64*)recipient_balance);
  // make sure sender has enough
  if (*(i64*)sender_balance < *(i64*)value)
    signalError();

  // adjust balances
  * (i64*)sender_balance -= * (i64*)value;
  * (i64*)recipient_balance += * (i64*)value;

  *(i64*)sender_balance = reverse_bytes(*(i64*)sender_balance);
  *(i64*)recipient_balance = reverse_bytes(*(i64*)recipient_balance);
  storageStore((i32ptr*)sender, (i32ptr*)sender_balance, 8);
  storageStore((i32ptr*)recipient, (i32ptr*)recipient_balance, 8);
}

void topUp() {
  // getPlayer
  getCaller((i32ptr*)sender);
  // getJoinValue
  getCallValue((i32ptr*)value);

  // get balances
  storageLoad((i32ptr*)sender, (i32ptr*)sender_balance);
  *(i64*)sender_balance = reverse_bytes(*(i64*)sender_balance);

  // adjust balances
  *(i64*)sender_balance += * (i64*)value;
  *(i64*)sender_balance = reverse_bytes(*(i64*)sender_balance);
  storageStore((i32ptr*)sender, (i32ptr*)sender_balance, 8);
}

// global data used in next function, will be allocated to WebAssembly memory
i32 selector[1] = {0};
void _main(void) {
}
