// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. convolve.cpp -o convolve
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. convolve.cpp
// cl version 19.35.32215 for x64

// https://www.jackcampbellsounds.com/2017/06/12/conv1.html
// CONVOLUTION REVERB PART 1 - OVERVIEW

// https://www.jackcampbellsounds.com/2017/06/12/conv2.html
// CONVOLUTION REVERB PART 2 - DIRECT-FORM CONVOLUTION

// https://www.jackcampbellsounds.com/2017/06/25/conv3.html
// CONVOLUTION REVERB PART 3 - FAST CONVOLUTION

// https://www.jackcampbellsounds.com/2019/01/24/simdsseboilerplate.html
// GETTING STARTED WITH SIMD INTRINSICS by Jack Campbell

#include "common.h"
#include "print.h"
#include "fill.h"
#include "speedup.h"

#if defined(__x86_64__) || defined(__i386__)
#include <emmintrin.h>
#endif

static bool debug = true;

static void convolve_1d_naive(const float *input, int n,
                              const float *kernel, int m,
                              float *output)
{
    assert(input && kernel && output && m < n);
    const auto size{ n - m + 1 };
    for (auto i = 0; i < size; ++i) {
        auto sum{ 0.f };
        for (auto j = 0; j < m; ++j) {
            const auto s = input[i + j];
            const auto k = kernel[j];
            sum += s * k;
        }
        output[i] = sum;
    }
}

#if 0
#if defined(__x86_64__) || defined(__i386__)
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
#endif

static void example(void (*f)(const float*, int, const float*, int, float*),
                    float *input, int n,
                    float *kernel, int m,
                    float *output)
{
    static auto counter = 1;
    std::cout << "Example " << counter++ << ": ";

    f(input, n, kernel, m, output);

    std::cout << std::endl;
    if (debug)
        print(output, n, "output");
}

// Sample Rate = Sampling Frequency = 44100 Hz
// Sample Period = 1 / 44100 = ~0.0227 ms
// Frame Size = 512
// Frame Duration = 512 / 44100 = ~11.61 ms

#define N 512
static float input[N];
static float output[N];

#define F 44100
#define G 15
static float gaussian[G];

#define S 8
static float sine[S];

#define C 4
static float cosine[C];

static void f1(void) { example(convolve_1d_naive, input, N, gaussian, G, output); }
static void f2(void) { example(convolve_1d_naive, input, N, sine, S, output); }
static void f3(void) { example(convolve_1d_naive, input, N, cosine, C, output); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    fill_sin(input, N);
    if (debug)
        print(input, N, "input");

    fill_gaussian(gaussian, F, G);
    print(gaussian, G, "gaussian kernel");

    fill_sin(sine, S);
    reverse(sine, S);
    print(sine, S, "sine kernel (reversed)");

    fill_cos(cosine, C);
    reverse(cosine, C);
    print(cosine, C, "cosine kernel (reversed)");

    for (const auto& f : examples)
        f();
}
