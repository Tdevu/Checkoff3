# Simple Encryption Using XOR

**Tanuj Devulapalli · COMP 211 (Systems Fundamentals), UNC Chapel Hill · Checkoff 3**

In COMP 211, we already touched on XOR when we used it to toggle flag bits, so for this checkoff I wanted to see how that same simple operation can be used to hide messages and how securely it does this, essentially the idea of encryption.

## What I looked into

I looked into the basic process of encrypting and decrypting and how plugging in the key does both. (plain ^ key = cipher & cipher ^ key = plain)

Why decrypting works. (x ^ k) ^ k = x ^ (k ^ k) = x ^ 0 = x

How to actually crack the code. This was fun to do as I realized that it's actually much simpler to think about, as there are such a low number of options for keys. (2^8 = 256)

Some C problems that occurred were the ideas of a ciphertext byte being 0. C reads this as the end of the word and breaks strlen and print("%s").

Lastly, I looked at when XOR is actually secure: when the key is truly random, as long as the message it is encrypting and is never reused.

## Things that surprised me

**XOR with a space flips a case**
A space is only 0x20, which only has a 5-bit set, and bit 5 is the only difference between lowercase and uppercase letters.

**Brute force fails with short messages**
Shorter messages can pick multiple keys that have the same tied score due to the small size.


**The key is the weak part, not XOR**
The difference in the complexities of the key, such as size, uniqueness, and how it can decrypt to other plausible words, creates the factors of how secure the encryption is.

## Code examples

All programs are in [`src/`](src). <!-- note on compiling with no warnings -->

| Program | What it shows |
|---|---|
| `xor_basic.c` | Encrypting and decrypting "Go Heels!" with the repeating key "UNC", using one function for both |
| `strlen_bug.c` | How a 0x00 byte in the ciphertext breaks strlen |
| `brute_force.c` | Cracking a single-byte XOR key by trying all 256 keys and scoring each output |
| `signed_char.c` | Why raw bytes should be unsigned char (0xE9 prints as ffffffe9 when signed) |

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

- I swapped in my own messages and keys ("Go Heels!", "UNC", "BEAT DUKE").
- I came up with my own null-byte case for strlen_bug.c ("Go Heels!" with key "Zo").
- I ran brute_force.c on a few different kinds of input (normal, all caps, really short) to see where it starts to fail.

## Presentation

Slides: [`slides/XOR_Encryption.pdf`](slides/XOR_Encryption.pdf)

## How I used AI

I learned this topic using Claude, where I started with the background and conceptual ideas, then progressed to seeing it in code. I was able to learn through reclarifying, looking through the examples and doing them on my own, and lastly explaining the same concepts back in my own words and checking if I missed anything.

- AI conversation: https://claude.ai/share/0920fbca-9b0d-495d-9d61-be8f3035d26b
- The code examples were written with help from Claude (Anthropic), and then I ran, modified, and tested them myself.

## Resources

See [`resources.md`](resources.md).
