/*
 * brute_force.c - Crack a single-byte XOR key by trying all 256 keys.
 *
 * For each key, decrypt the ciphertext and:
 *   - skip it if any decrypted byte is unprintable (wrong key)
 *   - otherwise score it: spaces and letters earn points
 * The highest-scoring key is the best guess.
 *
 * Compile: gcc -Wall -Wextra -o brute_force brute_force.c
 * Run:     ./brute_force
 *
 * Written with help from Claude (Anthropic).
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * Decrypt with one key and score the result.
 * Returns -1 if any byte is unprintable, so the caller can skip it.
 */
int score(const unsigned char* cipher, size_t len, unsigned char key) {
    int s = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = cipher[i] ^ key;
        if (!isprint(c)) {
            return -1; /* unprintable: not English text */
        }
        if (c == ' ') {
            s += 2; /* spaces are very common in English */
        } else if (isalpha(c)) {
            s += 1; /* letters are good too */
        }
    }
    return s;
}

int main(void) {
    /* Make a test ciphertext: encrypt a known message with a secret
     * key. In a real attack you would only have the ciphertext. */
    unsigned char cipher[] = "BEAT DUKE";
    size_t len = strlen((char*)cipher); /* measured before encrypting */
    unsigned char secret = 0x5A;
    for (size_t i = 0; i < len; i++) {
        cipher[i] ^= secret;
    }

    int best_key = -1;
    int best_score = -1;
    int skipped = 0;

    /* k must be an int: an unsigned char can't hold 256, so
     * "k < 256" would always be true and the loop would never end. */
    for (int k = 0; k < 256; k++) {
        int s = score(cipher, len, (unsigned char)k);
        if (s < 0) {
            skipped++; /* had unprintable bytes */
        } else if (s > best_score) {
            best_score = s;
            best_key = k;
        }
    }

    printf("keys tried:   256\n");
    printf("skipped:      %d (unprintable output)\n", skipped);

    if (best_key < 0) {
        printf("no printable candidate found\n");
        return 1;
    }

    printf("best key:     0x%02x (score %d)\n", best_key, best_score);
    printf("best guess:   ");
    for (size_t i = 0; i < len; i++) {
        putchar(cipher[i] ^ best_key);
    }
    printf("\n");
    return 0;
}
