// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// a.k.a Hamming Weight

// See: https://www.youtube.com/watch?v=Df-qEsWjzQw

// x86_64-pc-linux-gnu-gcc-10.2.0 -O3 -o popcount popcount.c
int popcount(uint32_t i) {
     i -= (i >> 1) & 0x55555555;
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

// Matt Godbolt: https://www.youtube.com/watch?v=w0sz5WbS5AM @ 39:49
// -O2 -march=haswell -Wall -Wextra -pedantic
// int countSetBits(unsigned int)
//   xor eax,eax
//   popcnt eax, edit
//   ret
int countSetBits(uint32_t n) {
    int count = 0;
    while (n != 0) {
        count++;
        n &= (n - 1);
    }
    return count;
}

// -O2 -march=haswell -Wall -Wextra -pedantic
// bool isPowerOfTwo_popcnt(uint32_t n)
//  test    edi, edi
//  je      .L5
//  popcnt  edi, edi
//  cmp     edi, 1
//  sete    al
//  ret
// .L5:
//  xor     eax, eax
//  ret
bool isPowerOfTwo_popcnt(uint32_t n) {
    return countSetBits(n) == 1;
}

// Matt Godbolt: https://www.youtube.com/watch?v=w0sz5WbS5AM @ 41:15
// bool isPowerOfTwo_blsr(uint32_t n)
//  xor     eax, eax
//  test    edi, edi
//  je      .L6
//  blsr    eax, edi
//  sete    al
// .L6:
//  ret
bool isPowerOfTwo_blsr(uint32_t n) {
    return n && (n & (n - 1)) == 0;
}

int main(int argc, char *argv[]) {
    uint32_t val = 0;

    if (argc > 1) {
        sscanf(argv[1], "%d", &val);
    } else {
        printf("Input a decimal value or 0 to end: ");
        if (scanf("%d", &val) != 1) {
            printf("scanf() ERROR");
            return -1;
        }
    }

    printf("Number of set bits in %d (0x%x)\n", val, val);

    printf("popcount() is %d\n", popcount(val));

    printf("countSetBits() is %d\n", countSetBits(val));

    printf("isPowerofTwo_popcnt() is %d\n", isPowerOfTwo_popcnt(val));

    printf("isPowerofTwo_blsr() is %d\n", isPowerOfTwo_blsr(val));

    return 0;
}
