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

#if defined(__ARM_NEON__) || defined(__ARM_NEON) || defined(__arm64__) || defined(__aarch64__)
#include <arm_neon.h>
float dot_f32_neon(const float* v1, const float* v2, size_t n)
{
    float32x4_t q1, q2;
    float32x4_t sum = vdupq_n_f32(0.f);
    assert(v1 && v2 && n > 0);
    assert(n % 4 == 0);
    do {
        q1 = vld1q_f32(&v1[i]);
        q2 = vld1q_f32(&v2[i]);
        sum = vmlaq_f32(sum, q1, q2);
    } while (n -= 4);
    return sum;
}
#endif

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
    #define N 16
    float zero[N] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    float one[N] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
    float v1[N] = { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f };
    float v2[N] = { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f };

    if (debug) {
        printf("v1: ");
        print_f32(v1, N);
        printf("\nv2: ");
        print_f32(v2, N);
        printf("\n");
    }

    printf("dot_f32: (%d) v1 . v2 = %f\n", N, dot_f32(v1, v2, N));
    printf("dot_f32: (%d) 1 . 1 = %f\n", N, dot_f32(one, one, N));
    printf("dot_f32: (%d) 0 . 0 = %f\n", N, dot_f32(zero, zero, N));

#if defined(__ARM_NEON__) || defined(__ARM_NEON) || defined(__arm64__) || defined(__aarch64__)
    printf("dot_f32_neon: (%d) v1 . v2 = %f\n", N, dot_f32_neon(v1, v2, N));
    printf("dot_f32_neon: (%d) 1 . 1 = %f\n", N, dot_f32_neon(one, one, N));
    printf("dot_f32_neon: (%d) 0 . 0 = %f\n", N, dot_f32_neon(zero, zero, N));
#endif

    return 0;
}