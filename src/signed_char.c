/*
 * signed_char.c - Why raw bytes should be unsigned char.
 *
 * 0xE9 is 1110 1001. Its top bit is 1, so as a signed char it means -23.
 * printf promotes a char to int, and -23 as a 32-bit int is
 * 0xFFFFFFE9 (sign extension copies the top bit into the new bits).
 * As an unsigned char it means 233, which becomes 0x000000E9.
 *
 * Compile: gcc -Wall -Wextra -o signed_char signed_char.c
 * Run:     ./signed_char
 *
 * Written with help from Claude (Anthropic).
 */

#include <stdio.h>

int main(void) {
    signed char   s = (signed char)0xE9;   /* same bits, read as -23 */
    unsigned char u = 0xE9;                /* same bits, read as 233 */

    printf("signed char:   %%d = %4d   %%02x = %02x\n", s, s);
    printf("unsigned char: %%d = %4d   %%02x = %02x\n", u, u);

    /* Fix for a signed char: cast to unsigned char before printing. */
    printf("signed, cast:  %%02x = %02x\n", (unsigned char)s);

    return 0;
}
