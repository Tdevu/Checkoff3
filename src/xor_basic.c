/*
 * xor_basic.c - Encrypt and decrypt a message with repeating-key XOR.
 *
 * Compile: gcc -Wall -Wextra -o xor_basic xor_basic.c
 * Run:     ./xor_basic
 *
 * Written with help from Claude (Anthropic).
 */

#include <stdio.h>
#include <string.h>

/*
 * XOR every byte of data with the matching key byte.
 * The key repeats: key[i % keylen] cycles K, E, Y, K, E, Y, ...
 * Calling this once encrypts; calling it again with the same key
 * decrypts, because (x ^ k) ^ k == x.
 */
void xor_crypt(unsigned char* data, size_t len, const unsigned char* key,
               size_t keylen) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[i % keylen];
    }
}

/* Print len bytes in hex, after a label. */
void print_hex(const char* label, const unsigned char* data, size_t len) {
    printf("%s", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char msg[] = "Go Heels!";
    const unsigned char key[] = "UNC";

    /* Measure lengths BEFORE encrypting: the ciphertext could
     * contain a 0x00 byte, which would make strlen stop early. */
    size_t len = strlen((char*)msg);
    size_t keylen = strlen((char*)key);

    print_hex("plain:     ", msg, len);

    xor_crypt(msg, len, key, keylen); /* encrypt */
    print_hex("cipher:    ", msg, len);

    xor_crypt(msg, len, key, keylen); /* decrypt (same call) */
    print_hex("decrypted: ", msg, len);

    printf("text:      %s\n", msg);
    return 0;
}
