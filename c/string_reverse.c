// SPDX-License-Identifier: MIT
// gcc -O3 string_reverse.c -o string_reverse
// gcc --version 11.3.0 on soho ubuntu 22.04

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_CAPACITY 1024
#define BUFFER_ALLOC    ((BUFFER_CAPACITY / sizeof(uint32_t)) + 2)

//   mov rd8, eax
//   bswap rd8
//   ret
static inline uint32_t swap32(uint32_t x) {
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) <<  8) |
           ((x & 0x00FF0000) >>  8) |
           ((x & 0xFF000000) >> 24);
}

static inline uint16_t swap16(uint16_t x) {
    return ((x & 0x00FF) <<  8) |
           ((x & 0xFF00) >>  8);
}

static void reverse8(uint8_t *head, int n) {
    uint8_t *tail, c;

    tail = head + n - 1;
    while (head < tail) {
        c = *head;
        *head++ = *tail;
        *tail-- = c;
    }
}

static void reverse16(uint16_t *head, int n) {
    uint16_t *tail, swap, even;

    tail = head + (n >> 1) - 1;
    even = (tail - head) & 1;
    while (head < tail) {
        swap = swap16(*head);
        *head++ = swap16(*tail);
        *tail-- = swap;
    }
    if (!even)
        reverse8((uint8_t *)head, sizeof(uint16_t));
}

static void reverse32(uint32_t *head, int n) {
    uint32_t *tail, swap, even;

    tail = head + (n >> 2) - 1;
    even = (tail - head) & 1;
    while (head < tail) {
        swap = swap32(*head);
        *head++ = swap32(*tail);
        *tail-- = swap;
    }
    if (!even)
        reverse16((uint16_t *)head, sizeof(uint32_t));
}

static inline void reverse(uint32_t *head, int n) {
    if ((n & 3) == 0)
        reverse32(head, n);
    else if ((n & 1) == 0)
        reverse16((uint16_t *)head, n);
    else
        reverse8((uint8_t *)head, n);
}

int main(int argc, char *argv[]) {
    uint32_t buffer[BUFFER_ALLOC];
    uint8_t *s = (uint8_t *)buffer;
    uint8_t c;
    int offset = 0;

    if (argc > 1 && argv[1][0])
        strncpy(s, argv[1], BUFFER_CAPACITY);
    else {
        printf("Input a string or just Enter to end: ");
        while (offset < BUFFER_CAPACITY) {
            c = getchar();
            if (c == '\n' && c != '\r')
                break;
            s[offset++] = c;
        }
        s[offset] = '\0';
    }

    printf("Original string: [%s]\n", s);
    reverse(buffer, strlen(s));
    printf("Reversed string: [%s]\n", s);
    
    return 0;
}
