// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. convolve.cpp -o convolve
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. convolve.cpp
// cl version 19.35.32215 for x64

// https://www.jackcampbellsounds.com/2019/01/24/simdsseboilerplate.html
// GETTING STARTED WITH SIMD INTRINSICS by Jack Campbell

#include "common.h"
#include "print.h"
#include "speedup.h"

#if defined(__x86_64__) || defined(__i386__)
#include <emmintrin.h>
#elif defined(__ARM_NEON__) || defined(__ARM_NEON) || defined(__arm64__) || defined(__aarch64__)
#include <arm_neon.h>
#endif

// https://www.jackcampbellsounds.com/2019/01/24/simdsseboilerplate.html

static void convolve_naive(float *inSig, size_t M,
                           float *inKernel, size_t N,
                           float *outSig)
{
    const size_t outLen = M + N - 1;
    for (size_t i = 0; i < outLen; i+=4)
    {
        float accumulator = 0;
        float prod;
        float sig;
        float kernel;
        for (size_t j = 0; j < N; ++j)
        {
            sig = inSig[i+j];
            kernel = inKernel[N - 1 - j];
            prod = sig * kernel;
            accumulator = accumulator + prod;
        }
        outSig[i] = accumulator;
    }
}

#if 0
// https://www.jackcampbellsounds.com/2019/01/24/simdsseboilerplate.html

static void convolve_simd_unaligned(float *inSig, size_t M,
                                    float *inKernel, size_t N,
                                    float *outSig)
{
    const size_t outLen = M + N - 1;
    // Preprocess the kernel:
    // reverse the kernel pre-loop and repeat each value across a 4-vector
    alignas(16) __m128 inKernelSIMD[N];
    for(int i=0; i<N; i++)
    {
        inKernelSIMD[i] = _mm_set1_ps(inKernel[N - i - 1]);
    }
    
    // pre-loop vars
    alignas(16) __m128 simd_sig;
    alignas(16) __m128 prod;
    alignas(16) __m128 accumulator;
    for (size_t i = 0; i < outLen; i+=4)
    {
        accumulator = _mm_setzero_ps();
        for (size_t j = 0; j < N; ++j)
        {
            simd_sig = _mm_loadu_ps(&inSig[i + j]);
            prod = _mm_mul_ps(simd_sig, inKernelSIMD[j]);
            accumulator = _mm_add_ps(accumulator, prod);
        }
        _mm_storeu_ps(&outSig[i], accumulator);
    }
}

// https://www.jackcampbellsounds.com/2019/01/24/simdsseboilerplate.html

static void convolve_simd(float *inSig, size_t M,
                          float *inKernel, size_t N,
                          float *outSig)
{
    const size_t outLen = M + N - 1;
    
    // preprocess the input
    // store offset versions of the input signal to allow for aligned loads
    for(int ii = 0; ii < 4; ii++)
    {
        int j = 0;
        for (size_t i = 0; i < M; i+=4)
        {
            inSignalSIMD[ii][j++] = _mm_set_ps(inSig[i+0+ii], 
                                               inSig[i+1+ii], 
                                               inSig[i+2+ii], 
                                               inSig[i+3+ii]);
        }
    }

    // Preprocess the kernel:
    // reverse the kernel pre-loop and repeat each value across a 4-vector
    alignas(16) __m128 inKernelSIMD[N];
    for(int i=0; i<N; i++)
    {
        inKernelSIMD[i] = _mm_set1_ps(inKernel[N - i - 1]);
    }
    
    // pre-loop vars
    alignas(16) __m128 accumulator;
    for (size_t i = 0; i < outLen; i+=4)
    {
        accumulator = _mm_setzero_ps();
        for (size_t j = 0; j < N; ++j)
        {
            // indexing gets real complicated!
            int index = i/4 + (int)(j*0.25); 
            accumulator = _mm_add_ps(accumulator, 
                                     _mm_mul_ps(inSignalSIMD[j%4][index], 
                                     inKernelSIMD[j]));
        }
        // still an unaligned store. room for improvement.
        _mm_storeu_ps(&outSig[i], accumulator);
    }
}
#endif

void example(void (*f)(float*, size_t, float*, size_t, float*),
             float *inSig, size_t M,
             float *inKernel, size_t N,
             float *outSig)
{
    static auto counter = 1;
    std::cout << "Example " << counter++ << ": ";

    f(inSig, M, inKernel, N, inSig);
}

void f1(void) { convolve_naive(nullptr, 0, nullptr, 0, nullptr); }
//void f2(void) { convolve_simd_unaligned(nullptr, 0, nullptr, 0, nullptr); }
//void f3(void) { convolve_simd(nullptr, 0, nullptr, 0, nullptr); }

std::vector<void (*)(void)> examples {
    f1
};

int main() {
    for (const auto& f : examples)
        f();
}
