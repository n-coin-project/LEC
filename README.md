# LivingEntropyCipher (LEC)

A Structure‑Independent Cipher with Evolving Entropy  
Reference Implementation (C Language)

---

## Overview

LivingEntropyCipher (LEC) is a symmetric encryption scheme that does not rely on mathematical hardness assumptions or random number generators.  
Instead, LEC uses a 100 KB evolving table that mutates irreversibly during encryption and decryption.

This reference implementation provides:

- Minimal and clean C implementation of the LEC core  
- Fully deterministic behavior matching the original C# prototype  
- Structure‑independent design with no algebraic attack surface  
- Built‑in table fingerprinting for synchronization and tamper detection  

LEC is a volume‑based cryptographic primitive where security emerges from the enormous state space of the evolving table.

---

## Key Features

- 100 KB evolving table (256^100000 state space)  
- History‑dependent irreversible mutation  
- No RNG required  
- No algebraic structure  
- Strong statistical properties:  
  - Entropy: 7.99 / 8.0  
  - Correlation: 0.0003  
- TableHash (FNV‑1a) for synchronization  
- Identical behavior to the original C# implementation  

---

## Files

### Documentation
- [doc/LivingEntropyCipher(English).pdf](doc/LivingEntropyCipher(English).pdf)
- [doc/LivingEntropyCipher(Japanese).pdf](doc/LivingEntropyCipher(Japanese).pdf)

### Source Code
- [src/lec.h](src/lec.h)
- [src/lec.c](src/lec.c)

---

## Directory Structure

~~~
LEC/
├─ doc/
│   ├─ LivingEntropyCipher(English).pdf
│   └─ LivingEntropyCipher(Japanese).pdf
│
└─ src/
    ├─ lec.h
    └─ lec.c
~~~

---

## Algorithm Summary

For each byte:

- Compute key from evolving table  
- XOR input with key  
- Mutate table using ciphertext  
- Advance pointer and index  

The table evolves every byte, ensuring:

- No state repeats  
- No reversible structure exists  
- No algebraic shortcut is possible  

---

## C API

### Initialization
~~~
int lec_init(LECContext* ctx, uint64_t seed, int jump, uint8_t mask);
~~~

### Encryption / Decryption
~~~
int lec_encrypt_buffer(LECContext* ctx, const uint8_t* in, uint8_t* out, size_t len);
int lec_decrypt_buffer(LECContext* ctx, const uint8_t* in, uint8_t* out, size_t len);
~~~

### Table Fingerprint
~~~
uint32_t lec_get_fingerprint(const LECContext* ctx);
~~~

---

## Example Usage (C)

~~~
#include "lec.h"
#include <stdio.h>

int main() {
    LECContext ctx;
    lec_init(&ctx, 123456789ULL, 7, 0x55);

    uint8_t input[] = "Hello LEC!";
    uint8_t encrypted[64];
    uint8_t decrypted[64];

    lec_encrypt_buffer(&ctx, input, encrypted, 10);

    lec_init(&ctx, 123456789ULL, 7, 0x55);
    lec_decrypt_buffer(&ctx, encrypted, decrypted, 10);

    printf("Decrypted: %s\n", decrypted);
    return 0;
}
~~~

---

## Security Notes

- Not based on algebraic hardness  
- No known quantum algorithm can exploit its structure  
- Security derives from:  
  - irreversible table evolution  
  - massive state space  
  - history‑dependent mutation  

---

## License

MIT License (recommended)

---

## Reference

This implementation corresponds to the paper:  
“LivingEntropyCipher (LEC): A Structure‑Independent Cipher with Evolving Entropy”  
NS (2026)
