// SPDX-License-Identifier: MIT

#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>

static void print_int(int *v, size_t n)
{
    int i = 0;

    printf("(%zd) [ ", n);
    for (;;) {
        printf("%d", *v++);
        if (++i == n)
            break;
        printf(", ");
    }
    printf(" ]");
}

static void print_f32(float *v, size_t n)
{
    int i = 0;

    printf("(%zd) [ ", n);
    for (;;) {
        printf("%f", *v++);
        if (++i == n)
            break;
        printf(", ");
    }
    printf(" ]");
}

#endif // _PRINT_H_
