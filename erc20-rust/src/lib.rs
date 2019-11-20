#![no_std]

// cargo +nightly rustc -- -Z unstable-options --pretty=expanded > demacroed.rs

#![no_std]
#![no_main]

extern crate elrond_wasm;

use elrond_wasm as ew;
use elrond_wasm::BigInt;


static TOTAL_SUPPLY_KEY: [u8; 32] = [0u8; 32];

static APPROVE_EVENT:  [u8; 32] = [0x71,0x34,0x69,0x2B,0x23,0x0B,0x9E,0x1F,0xFA,0x39,0x09,0x89,0x04,0x72,0x21,0x34,0x15,0x96,0x52,0xB0,0x9C,0x5B,0xC4,0x1D,0x88,0xD6,0x69,0x87,0x79,0xD2,0x28,0xFF];
static TRANSFER_EVENT: [u8; 32] = [0xF0,0x99,0xCD,0x8B,0xDE,0x55,0x78,0x14,0x84,0x2A,0x31,0x21,0xE8,0xDD,0xFD,0x43,0x3A,0x53,0x9B,0x8C,0x9F,0x14,0xBF,0x31,0xEB,0xF1,0x08,0xD1,0x2E,0x61,0x96,0xE9];


// Generates a balance key for some address.
// Used to map balances with their owners.
fn balance_key(address: &[u8; 32]) -> [u8; 32] {
  let mut key = [0u8; 32];
  // reserve one byte of the key to indicate key type
  // "1" is for balance keys
  key[0] = 1;

  // the last 2 bytes of the address are only used to identify the shard, 
  // so they are disposable when constructing the key
  for i in 0..30 {
    key[2+i] = address[i];
  }

  key
}

fn perform_transfer(sender: &[u8; 32], recipient: &[u8; 32], amount: BigInt) {
    // load sender balance
    let sender_balance_key = balance_key(&sender);
    let mut sender_balance = ew::storage_load_big_int(&sender_balance_key);
  
    // check if enough funds
    if BigInt::compare(&amount, &sender_balance) > 0 {
      ew::signal_error();
      return;
    }
  
    // update sender balance
    sender_balance -= &amount;
    ew::storage_store_big_int(&sender_balance_key, &sender_balance);
  
    // load & update receiver balance
    let rec_balance_key = balance_key(&recipient);
    let mut rec_balance = ew::storage_load_big_int(&rec_balance_key);
    rec_balance += &amount;
    ew::storage_store_big_int(&rec_balance_key, &rec_balance);
  
    // log operation
    save_log_with_3_topics(&TRANSFER_EVENT, &sender, &recipient, &amount);
}

#[owasm_abi_derive::contract]
trait TokenContract {
    fn constructor(&mut self, total_supply: &BigInt) {
        let sender = ew::get_caller();

        // save total supply
        ew::storage_store_big_int(&TOTAL_SUPPLY_KEY, &total_supply);

        // sender balance <- total supply
        let balance_key = balance_key(&sender);
        ew::storage_store_big_int(&balance_key, &total_supply);
    }

    #[constant]
    fn balanceOf(&mut self, subject: &[u8; 32]) -> BigInt {
        // load balance
        let balance_key = balance_key(&subject);
        let balance = ew::storage_load_big_int(&balance_key);

        // return balance as big int
        balance
    }

    #[constant]
    fn totalSupply(&mut self) -> BigInt {
        let total_supply = ew::storage_load_big_int(&TOTAL_SUPPLY_KEY);
        total_supply
    }

    fn transfer(&mut self, to: &[u8; 32], amount: BigInt) -> bool {
        // sender is the caller
        let sender = ew::get_caller();

        if BigInt::compare(&amount, &BigInt::from_i64(0)) < 0 {
            ew::signal_error();
            false;
        }
        
        perform_transfer(&sender, &recipient, amount);
        true
    }

}

