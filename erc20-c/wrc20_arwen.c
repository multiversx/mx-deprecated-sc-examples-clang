#include "elrond/context.h"
#include "elrond/bigInt.h"

// global data used in functions, will be statically allocated to WebAssembly memory
byte sender[32]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte recipient[32]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte caller[32]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte currentKey[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void computeTotalSupplyKey(byte *destination) {
  // only the total supply key starts with byte "0"
  for (int i = 0; i < 32; i++) {
    destination[i] = 0;
  }
}

void computeBalanceKey(byte *destination, byte *address) {
  // reserve one byte of the key to indicate key type
  // "1" is for balance keys
  destination[0] = 1;
  destination[1] = 0;

  // the last 2 bytes of the address are only used to identify the shard, 
  // so they are disposable when constructing the key
  for (int i = 0; i < 30; i++) {
    destination[2+i] = address[i];
  }
}

void computeAllowanceKey(byte *destination, byte *from, byte* to) {
  // reserve one byte of the key to indicate key type
  // "2" is for allowance keys
  destination[0] = 2;

  // Note: in smart contract addresses, the first 10 bytes are all 0
  // therefore we read from byte 10 onwards to provide more significant bytes
  // and to minimize the chance for collisions
  // TODO: switching to a hash instead of a concatenation of addresses might make it safer
  for (int i = 0; i < 15; i++) {
    destination[1+i] = from[10+i];
  }
  for (int i = 0; i < 16; i++) {
    destination[16+i] = to[10+i];
  }
}

// constructor function
// is called immediately after the contract is created
// will set the fixed global token supply and give all the supply to the creator
void init() {
  if (getNumArguments() != 1) {
    signalError();
    return;
  }

  getCaller(sender);
  bigInt totalSupply = bigIntNew(0);
  bigIntgetArgument(0, totalSupply);

  // set total supply
  computeTotalSupplyKey(currentKey);
  bigIntstorageStore(currentKey, totalSupply);

  // sender balance <- total supply
  computeBalanceKey(currentKey, sender);
  bigIntstorageStore(currentKey, totalSupply);
}

// getter function: retrieves total token supply
void totalSupply() {
  if (getNumArguments() != 0) {
    signalError();
    return;
  }
  
  // load total supply from storage
  computeTotalSupplyKey(currentKey);
  bigInt totalSupply = bigIntNew(0);
  bigIntstorageLoad(currentKey, totalSupply);

  // return total supply as big int
  bigIntFinish(totalSupply);
}

// getter function: retrieves balance for an account
void balanceOf() {
  if (getNumArguments() != 1) {
    signalError();
    return;
  }

  // argument: account to get the balance for
  getArgument(0, caller); 

  // load balance
  computeBalanceKey(currentKey, caller);
  bigInt balance = bigIntNew(0);
  bigIntstorageLoad(currentKey, balance);

  // return balance as big int
  bigIntFinish(balance);
}

// getter function: retrieves allowance granted from one account to another
void allowance() {
  if (getNumArguments() != 2) {
    signalError();
    return;
  }

  // 1st argument: owner
  getArgument(0, sender);

  // 2nd argument: spender
  getArgument(1, recipient);

  // get allowance
  computeAllowanceKey(currentKey, sender, recipient);
  bigInt allowance = bigIntNew(0);
  bigIntstorageLoad(currentKey, allowance);

  // return allowance as big int
  bigIntFinish(allowance);
}

// transfers tokens from sender to another account
void transferToken() {
  if (getNumArguments() != 2) {
    signalError();
    return;
  }

  // sender is the caller
  getCaller(sender);

  // 1st argument: recipient
  getArgument(0, recipient);

  // 2nd argument: amount
  bigInt amount = bigIntNew(0);
  bigIntgetArgument(1, amount);
  if (bigIntCmp(amount, bigIntNew(0)) < 0) {
    signalError();
    return;
  }

  // load sender balance
  computeBalanceKey(currentKey, sender);
  bigInt senderBalance = bigIntNew(0);
  bigIntstorageLoad(currentKey, senderBalance);

  // check if enough funds
  if (bigIntCmp(amount, senderBalance) > 0) {
    signalError();
    return;
  }

  // update sender balance
  bigIntSub(senderBalance, senderBalance, amount);
  bigIntstorageStore(currentKey, senderBalance);

  // load & update receiver balance
  computeBalanceKey(currentKey, recipient);
  bigInt receiverBalance = bigIntNew(0);
  bigIntstorageLoad(currentKey, receiverBalance);
  bigIntAdd(receiverBalance, receiverBalance, amount);
  bigIntstorageStore(currentKey, receiverBalance);

  // return "true"
  int64finish(1); 
}

// sender allows beneficiary to use given amount of tokens from sender's balance
// it will completely overwrite any previously existing allowance from sender to beneficiary
void approve() {
  if (getNumArguments() != 2) {
    signalError();
    return;
  }

  // sender is the caller
  getCaller(sender);

  // 1st argument: spender (beneficiary)
  getArgument(0, recipient);

  // 2nd argument: amount (should not be negative)
  bigInt amount = bigIntNew(0);
  bigIntgetArgument(1, amount);
  if (bigIntCmp(amount, bigIntNew(0)) < 0) {
    signalError();
    return;
  }

  // store allowance
  computeAllowanceKey(currentKey, sender, recipient);
  bigIntstorageStore(currentKey, amount);

  // return "true"
  int64finish(1); 
}


// caller uses allowance to transfer funds between 2 other accounts
void transferFrom() {
   if (getNumArguments() != 3) {
    signalError();
    return;
  }

  // save caller
  getCaller(caller);

  // 1st argument: sender
  getArgument(0, sender);

  // 2nd argument: recipient
  getArgument(1, recipient);

  // 3rd argument: amount
  bigInt amount = bigIntNew(0);
  bigIntgetArgument(2, amount);
  if (bigIntCmp(amount, bigIntNew(0)) < 0) {
    signalError();
    return;
  }

  // load allowance
  computeAllowanceKey(currentKey, sender, caller);
  bigInt allowance = bigIntNew(0);
  bigIntstorageLoad(currentKey, allowance);

  // amount should not exceed allowance
  if (bigIntCmp(amount, allowance) > 0) {
    signalError();
    return;
  }

  // update allowance
  bigIntSub(allowance, allowance, amount);
  bigIntstorageStore(currentKey, allowance);

  // load sender balance
  computeBalanceKey(currentKey, sender);
  bigInt senderBalance = bigIntNew(0);
  bigIntstorageLoad(currentKey, senderBalance);

  // check if enough funds
  if (bigIntCmp(amount, senderBalance) > 0) {
    signalError();
    return;
  }

  // update sender balance
  bigIntSub(senderBalance, senderBalance, amount);
  bigIntstorageStore(currentKey, senderBalance);

  // load & update receiver balance
  computeBalanceKey(currentKey, recipient);
  bigInt receiverBalance = bigIntNew(0);
  bigIntstorageLoad(currentKey, receiverBalance);
  bigIntAdd(receiverBalance, receiverBalance, amount);
  bigIntstorageStore(currentKey, receiverBalance);

  // return "true"
  int64finish(1); 
}

// global data used in next function, will be allocated to WebAssembly memory
i32 selector[1] = {0};
void _main(void) {
}
