#include "elrond/context.h"
#include "elrond/bigInt.h"
#include "elrond/util.h"

STORAGE_KEY(CREATOR_ADDRESS);
STORAGE_KEY(TOTAL_AMOUNT);

void _init_creator();
void _init_total_amount();
void _check_only_creator();

void init() {
    CHECK_NUM_ARGS(0);
    CHECK_NOT_PAYABLE();

    _init_creator();
    _init_total_amount();
}

void _init_creator() {
    byte creatorAddress[32];
    getCaller(creatorAddress);
    storageStore(CREATOR_ADDRESS_KEY, CREATOR_ADDRESS_KEY_LEN, creatorAddress, 32);
}

void _init_total_amount() {
    bigInt totalAmount = bigIntNew(0);
    bigIntSetInt64(totalAmount, 0);
    bigIntStorageStoreUnsigned(TOTAL_AMOUNT_KEY, TOTAL_AMOUNT_KEY_LEN, totalAmount);
}

void deposit() {
    _check_only_creator();

    // bigInt callValue = bigIntNew(0);
    // bigIntGetCallValue(callValue);
    // void getCaller(byte *callerAddress);
}

void _check_only_creator() {

}

void getTotalAmount() {
    bigInt totalAmount = bigIntNew(0);
    bigIntStorageLoadUnsigned(TOTAL_AMOUNT_KEY, TOTAL_AMOUNT_KEY_LEN, totalAmount);
    bigIntFinishUnsigned(totalAmount);
}