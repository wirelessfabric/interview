// SPDX-License-Identifier: MIT
// g++ -std=c++20 -O3 -I.. -mavx convolve.cpp -o convolve
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. convolve.cpp
// cl version 19.35.32215 for x64

// https://www.jackcampbellsounds.com/2017/06/12/conv1.html
// CONVOLUTION REVERB PART 1 - OVERVIEW by Jack Campbell

// https://www.jackcampbellsounds.com/2017/06/12/conv2.html
// CONVOLUTION REVERB PART 2 - DIRECT-FORM CONVOLUTION by Jack Campbell

// https://www.jackcampbellsounds.com/2017/06/25/conv3.html
// CONVOLUTION REVERB PART 3 - FAST CONVOLUTION by Jack Campbell

// https://www.jackcampbellsounds.com/2019/01/24/simdsseboilerplate.html
// GETTING STARTED WITH SIMD INTRINSICS by Jack Campbell

// https://thewolfsound.com/convolution-the-secret-behind-filtering/
// The Convolution Series by Jan Wilczek

// https://www.youtube.com/playlist?list=PLJLybGgHYEIdZ6uXe3D_U5tsoSbN22g1Q
// Convolution videos on DSP playlist by Jan Wilczek @ WolfSoundAudio

#include "common.h"
#include "print.h"
#include "utils.h"
#include "fill.h"
#include "speedup.h"

#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#define MM_INTRIN
#elif defined(_M_IX86) || defined(_M_X64)
#include <intrin.h>
#define MM_INTRIN
#endif

static bool debug = false;

static void convolve_1d_valid_mode_naive(
    const float *input, int n,
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

    fill(&output[size], 0.f, m - 1);
}

#ifdef MM_INTRIN
static void convolve_1d_valid_mode_simd_unaligned(
    const float *input, int n,
    const float *kernel, int m,
    float *output)
{
    assert(input && kernel && output && m < n);
    const auto size{ n - m + 1 };

#ifdef __GNUC__
    alignas(16) __m128 simd_kernel[m];
#else
    auto simd_kernel{ std::make_unique<__m128[]>(m) };
#endif
    for (auto i=0; i < m; ++i)
        simd_kernel[i] = _mm_set1_ps(kernel[i]);
    
    alignas(16) __m128 simd_input;
    alignas(16) __m128 product;
    alignas(16) __m128 sum;
    for (auto i = 0; i < size; i += 4) {
        sum = _mm_setzero_ps();
        for (auto j = 0; j < m; ++j) {
            simd_input = _mm_loadu_ps(&input[i + j]);
            product = _mm_mul_ps(simd_input, simd_kernel[j]);
            sum = _mm_add_ps(sum, product);
        }
        _mm_storeu_ps(&output[i], sum);
    }

    fill(&output[size], 0.f, m - 1);
}
#endif

#ifdef __GNUC__
#ifdef MM_INTRIN
static void convolve_1d_valid_mode_simd(
    const float *input, int n,
    const float *kernel, int m,
    float *output)
{
    assert(input && kernel && output && m < n);
    const auto size{ n - m + 1 };
    
    // preprocess the input
    // store offset versions of the input signal to allow for aligned loads
    alignas(16) __m128 simd_input[4][n];
    for (int k=0; k < 4; ++k) {
        int j = 0;
        for (int i=0; i < n; i += 4) {
            simd_input[k][j++] = _mm_set_ps(input[i+0+k], 
                                            input[i+1+k], 
                                            input[i+2+k], 
                                            input[i+3+k]);
        }
    }

    alignas(16) __m128 simd_kernel[m];
    for (int i=0; i < m; ++i)
        simd_kernel[i] = _mm_set1_ps(kernel[i]);
    
    alignas(16) __m128 sum;
    for (int i=0; i < size; i += 4) {
        sum = _mm_setzero_ps();
        for (int j=0; j < m; ++j) {
            // indexing gets real complicated!
            int k = i / 4 + (int)(j * 0.25);
            sum = _mm_add_ps(sum, _mm_mul_ps(simd_input[j & 3][k], simd_kernel[j]));
        }
        // still an unaligned store. room for improvement.
        _mm_storeu_ps(&output[i], sum);
    }

    fill(&output[size], 0.f, m - 1);
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

    auto t0{ std::chrono::high_resolution_clock::now() };
    f(input, n, kernel, m, output);
    auto t1{ std::chrono::high_resolution_clock::now() };

    std::chrono::duration<double> elapsed{ (t1 - t0) * 1000000.0 };
    std::cout << "\ttime " << elapsed.count() << "us";
    std::cout << "\tmean " << mean(output, n) << std::endl;
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
static float random01[N];
static float randomzc[N];

#define FREQ 44100.f
#define G 15
static float gaussian[G];

#define S 8
static float sine[S];

#define C 4
static float cosine[C];

static void f1(void) { example(convolve_1d_valid_mode_naive, input, N, gaussian, G, output); }
static void f2(void) { example(convolve_1d_valid_mode_naive, input, N, sine, S, output); }
static void f3(void) { example(convolve_1d_valid_mode_naive, input, N, cosine, C, output); }
static void f4(void) { example(convolve_1d_valid_mode_naive, random01, N, gaussian, G, output); }
static void f5(void) { example(convolve_1d_valid_mode_naive, randomzc, N, gaussian, G, output); }

#ifdef MM_INTRIN
static void m1(void) { example(convolve_1d_valid_mode_simd_unaligned, input, N, gaussian, G, output); }
static void m2(void) { example(convolve_1d_valid_mode_simd_unaligned, input, N, sine, S, output); }
static void m3(void) { example(convolve_1d_valid_mode_simd_unaligned, input, N, cosine, C, output); }
static void m4(void) { example(convolve_1d_valid_mode_simd_unaligned, random01, N, gaussian, G, output); }
static void m5(void) { example(convolve_1d_valid_mode_simd_unaligned, randomzc, N, gaussian, G, output); }
#ifdef __GNUC__
static void g1(void) { example(convolve_1d_valid_mode_simd, input, N, gaussian, G, output); }
static void g2(void) { example(convolve_1d_valid_mode_simd, input, N, sine, S, output); }
static void g3(void) { example(convolve_1d_valid_mode_simd, input, N, cosine, C, output); }
static void g4(void) { example(convolve_1d_valid_mode_simd, random01, N, gaussian, G, output); }
static void g5(void) { example(convolve_1d_valid_mode_simd, randomzc, N, gaussian, G, output); }
#endif
#endif

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5
#ifdef MM_INTRIN
    ,
    m1, m2, m3, m4, m5
#ifdef __GNUC__
    ,
    g1, g2, g3, g4, g5
#endif
#endif
};

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "-d") == 0)
        debug = true;

    fill(input, sinf, N);
    if (debug)
        print(input, N, "input");

    fill_random(random01, 0.f, 1.f, N);
    if (debug)
        print(random01, N, "random01");
    printf("random01 mean %f\n", mean(random01, N));

    fill_random(randomzc, -0.5f, 0.5f, N);
    if (debug)
        print(randomzc, N, "randomzc");
    printf("randomzc mean %f\n", mean(randomzc, N));

    fill_gaussian(gaussian, FREQ, G);
    if (debug)
        print(gaussian, G, "gaussian kernel");
    flip(gaussian, G);
    print(gaussian, G, "gaussian kernel (flipped)");

    fill(sine, sinf, S);
    if (debug)
        print(sine, S, "sine kernel");
    flip(sine, S);
    print(sine, S, "sine kernel (flipped)");

    fill(cosine, cosf, C);
    if (debug)
        print(cosine, S, "cosine kernel");
    flip(cosine, C);
    print(cosine, C, "cosine kernel (flipped)");

    for (const auto& f : examples)
        f();
}
