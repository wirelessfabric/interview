// SPDX-License-Identifier: MIT
//
// gcc -O3 dot_product.c -o dot_product
// gcc --version 11.3.0 on soho ubuntu 22.04
//
// clang -O3 dot_product.c -o dot_product
// Apple clang version 14.0.0 (clang-1400.0.29.202) on Apple M1 macOS Ventura 13.2.1
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc dot_product.c
// cl version 19.35.32215 for x64

#include "common.h"
#include "print.h"

static bool debug = true;

float dot_f32(const float* v1, const float* v2, size_t n)
{
    float sum = 0.f;
    assert(v1 && v2 && n > 0);
    do {
        const float f1 = *v1++;
        const float f2 = *v2++;
        sum += f1 * f2;
    } while (--n);
    return sum;
}

int main(int argc, char *argv[])
{
    #define N 10
    float zero[N] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    float one[N] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
    float v1[N] = { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
    float v2[N] = { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

    printf("dot_f32: (%d) 0 . 0 = %f\n", N, dot_f32(zero, zero, N));
    printf("dot_f32: (%d) 1 . 1 = %f\n", N, dot_f32(one, one, N));

    if (debug) {
        printf("v1: ");
        print_f32(v1, N);
        printf("\nv2: ");
        print_f32(v2, N);
        printf("\n");
    }

    printf("dot_f32: (%d) v1 . v2 = %f\n", N, dot_f32(v1, v2, N));

    return 0;
}
