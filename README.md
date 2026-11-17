# Simple Encryption Using XOR

**Tanuj Devulapalli · COMP 211 (Systems Fundamentals), UNC Chapel Hill · Checkoff 3**

In COMP 211 we use `^` with a mask to toggle bits. For this checkoff I explored how that same operator can hide a message, and how easily that kind of encryption breaks.

## What I explored

- **How XOR encryption works.** `cipher = plain ^ key` and `plain = cipher ^ key`. Encrypting and decrypting are the same operation with the same key.
- **Why decrypting works.** `(x ^ k) ^ k = x ^ (k ^ k) = x ^ 0 = x`. The key is a mask: every 1 bit flips a data bit, and flipping it a second time restores it.
- **How to break it.** A one-byte key has only 2⁸ = 256 possible values, so a program can try every one and pick the output that looks most like English.
- **C pitfalls.** A ciphertext byte can be `0x00`, which breaks `strlen` and `printf("%s")`. A signed `char` with its top bit set sign-extends when printed.
- **When XOR is secure.** The one-time pad: a truly random key, as long as the message, never reused.

## Most interesting discoveries

1. **XOR with a space flips a letter's case.** A space is `0x20` (only bit 5 set), and bit 5 is the only difference between `'a'` (`0110 0001`) and `'A'` (`0100 0001`).
2. **Brute force doesn't always work on short messages.** `"BEAT DUKE"` cracked correctly, but `"hi"` picked the wrong key, because with only 2 letters many keys tie on score.
3. **Ciphertext is raw bytes, not a string.** With `"Go Heels!"` and key `"Zo"`, `'o' ^ 'o' = 0x00`, so `strlen` reports 1 instead of 9 and decrypting with that length gives back only `"G"`.
4. **XOR itself isn't the weak part. The key is.** With a one-time pad, the ciphertext `01 06 17 69` decrypts to `UNC!` with one key and to `DUKE` with another, so an attacker learns nothing. Modern ciphers like AES-CTR and ChaCha20 still use XOR; they generate a long keystream from a short key.

## Code examples

All programs are in [`src/`](src). Each one compiles with no warnings.

| Program | What it shows |
|---|---|
| `xor_basic.c` | Encrypting and decrypting `"Go Heels!"` with the repeating key `"UNC"`, using one function for both |
| `strlen_bug.c` | A `0x00` byte in the ciphertext breaks `strlen` |
| `brute_force.c` | Cracking a single-byte XOR key by trying all 256 keys and scoring each output |
| `signed_char.c` | Why raw bytes should be `unsigned char` (`0xE9` prints as `ffffffe9` when signed) |

### How to run

```bash
cd src
gcc -Wall -Wextra -o xor_basic xor_basic.c && ./xor_basic
gcc -Wall -Wextra -o strlen_bug strlen_bug.c && ./strlen_bug
gcc -Wall -Wextra -o brute_force brute_force.c && ./brute_force
gcc -Wall -Wextra -o signed_char signed_char.c && ./signed_char
```

### Example output

`xor_basic`
```
plain:     47 6f 20 48 65 65 6c 73 21
cipher:    12 21 63 1d 2b 26 39 3d 62
decrypted: 47 6f 20 48 65 65 6c 73 21
text:      Go Heels!
```

`strlen_bug`
```
cipher:          1d 00 7a 27 3f 0a 36 1c 7b
saved length:    9
strlen(cipher):  1   <- stops at the 0x00 byte

--- decrypt with strlen(cipher) ---
text:  G

--- decrypt with saved length ---
text:  Go Heels!
```

`brute_force`
```
keys tried:   256
skipped:      192 (unprintable output)
best key:     0x5a (score 10)
best guess:   BEAT DUKE
```

`signed_char`
```
signed char:   %d =  -23   %02x = ffffffe9
unsigned char: %d =  233   %02x = e9
signed, cast:  %02x = e9
```

### What I changed and tested

- Used my own messages and keys (`"Go Heels!"`, `"UNC"`, `"BEAT DUKE"`).
- Designed my own null-byte case for `strlen_bug.c` (`"Go Heels!"` with key `"Zo"`).
- Tested `brute_force.c` on different inputs (normal, all caps, very short) to find where it fails.

## Presentation

Slides: [`slides/XOR_Encryption.pdf`](slides/XOR_Encryption.pdf)

## How I used AI

I learned this topic through a conversation with Claude, where I tested its claims by running code, explained concepts back in my own words, and corrected it when it misunderstood me.

- AI conversation: https://claude.ai/share/0920fbca-9b0d-495d-9d61-be8f3035d26b
- The code examples were written with help from Claude (Anthropic), then run, modified, and tested by me.

## Resources

See [`resources.md`](resources.md).
