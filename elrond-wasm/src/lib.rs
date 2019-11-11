
#![no_std]
#![allow(dead_code)]

mod ext;
mod ext_int64;
mod big_int;

pub use ext::*;
pub use ext_int64::*;
pub use big_int::*;

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
