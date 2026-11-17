/*
 * strlen_bug.c - Show why strlen must not be used on ciphertext.
 *
 * With message "Go Heels!" and key "Zo", the plaintext 'o' at index 1
 * meets key byte 'o', and 'o' ^ 'o' == 0x00.  strlen treats that 0x00
 * as the end of the string, so it reports 1 instead of 9.
 *
 * Compile: gcc -Wall -Wextra -o strlen_bug strlen_bug.c
 * Run:     ./strlen_bug
 *
 * Written with help from Claude (Anthropic).
 */

#include <stdio.h>
#include <string.h>

/* Same XOR function as xor_basic.c: encrypts and decrypts. */
void xor_crypt(unsigned char* data, size_t len, const unsigned char* key,
               size_t keylen) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[i % keylen];
    }
}

void print_hex(const char* label, const unsigned char* data, size_t len) {
    printf("%s", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(void) {
    const unsigned char key[] = "Zo";
    size_t keylen = strlen((char*)key);

    /* Two copies of the same message: one for the buggy way,
     * one for the correct way. */
    unsigned char bad[] = "Go Heels!";
    unsigned char good[] = "Go Heels!";

    /* CORRECT: save the length once, before encrypting. */
    size_t saved_len = strlen((char*)good);

    xor_crypt(bad, saved_len, key, keylen);
    xor_crypt(good, saved_len, key, keylen);

    print_hex("cipher:          ", good, saved_len);
    printf("saved length:    %zu\n", saved_len);
    printf("strlen(cipher):  %zu   <- stops at the 0x00 byte\n",
           strlen((char*)bad));

    /* BUG: re-measure the length from the ciphertext. */
    size_t bad_len = strlen((char*)bad);
    xor_crypt(bad, bad_len, key, keylen); /* only decrypts 1 byte */

    /* RIGHT: decrypt using the length saved before encrypting. */
    xor_crypt(good, saved_len, key, keylen); /* decrypts all 9 bytes */

    printf("\n--- decrypt with strlen(cipher) ---\n");
    print_hex("bytes: ", bad, saved_len);
    printf("text:  %s\n", bad); /* %s also stops at the 0x00 */

    printf("\n--- decrypt with saved length ---\n");
    print_hex("bytes: ", good, saved_len);
    printf("text:  %s\n", good);

    return 0;
}
