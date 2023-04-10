// SPDX-License-Identifier: MIT
// gcc -O3 counter.c -o counter
// gcc --version 11.3.0 on soho ubuntu 22.04

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool debug = true;

// [ 1, 2, ,3, ... 100, 100, 100, 103 ]
// input ( 1 ) ==> 1
// input (100) ==> 3
// inupt (-1) ==> 0

// [ 1, 2, 3,100, 100, 100, 103 ]

size_t counter(int *data, size_t data_count, int target)
{
    size_t count = 0;
    
    if (!data || !data_count)
        return 0;

    while (data_count && *data != target) {
        data++;
        data_count--;
    }

    while (data_count && *data++ == target) {
        count++;
        data_count--;
    }

    return count;
}

size_t sum_progression(size_t n)
{
    return n * (n + 1) / 2;
}

int *generate_data(int n, size_t *data_count) // [1, 2, 2, 3, 3, 3, ...]
{
    size_t count, size;
    int *data, *fill;
    int i, j;

    count = sum_progression(n);
    if (!count)
        return NULL;

    size = count * sizeof(int);
    data = malloc(size);
    if (!data)
        return NULL;
    
    fill = data;
    for (i=0; i < n;) {
        i++;
        for (j=0; j < i; j++)
            *fill++ = i;
    }

    if (data_count)
        *data_count = count;

    return data;
}

void print_data(int *data, size_t data_count)
{
    size_t size;
    int i = 0;

    printf("(%ld)[ ", data_count);
    for (;;) {
        printf("%d", *data++);
        if (++i == data_count)
            break;
        printf(", ");
    }
    printf(" ]");
}

int main(int argc, char *argv[])
{
    int *data;
    size_t data_count;
    int max = 10;
    int n, m;

    if (argc > 1)
        max = atoi(argv[1]);

    for (n=1; n < max; n++) {
        printf("Generate %d series... ", n);
        data = generate_data(n, &data_count);
        if (data && data_count) {
            if (debug)    
                print_data(data, data_count);
            for (m=0; m < max; m++) {
                size_t found = counter(data, data_count, m);
                if ((found != m) && (m != 0 && m < n))
                    printf("FAIL: found %ld of %d\n", found, m);
            }
            free(data);
        }
        printf("\n");
    }

    return 0;
}
