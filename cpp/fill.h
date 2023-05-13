// SPDX-License-Identifier: MIT

#ifndef FILL_H_
#define FILL_H_

#include <random>
#include <numbers>

template <typename T>
static T slice(int n) {
    constexpr auto pie{ (T)(2.0 * std::numbers::pi) };
    return pie / static_cast<T>(n);
}

static void fill(float* v, float (*f)(float), int n) {
    assert(v && f && n > 0);
    auto w{ 0.f };
    const auto df{ slice<float>(n) };
    do { *v++ = f(w); w += df; } while (--n);
}

static void fill(float* v, float f, int n) {
    assert(v && n > 0);
    do { *v++ = f; } while (--n);
}

static void fill_gaussian(float* v, float fs, int n) {
    assert(v && fs > 0 && n > 0);
    const auto tau{ (float)n / ((float)(2.0 * std::numbers::pi) * fs) };
    const auto dt{ 1.f / fs };
    auto t{ -dt * (float)(n - 1) * 0.5f };
    do {
        const auto k{ t / tau };
        *v++ = expf(-k * k);
        t += dt;
    } while (--n);
}

static std::mt19937& prng(void);

static void fill_random(float* v, float min, float max, int n) {
    std::uniform_real_distribution<float> d(min, max);
    do *v++ = d(prng()); while (--n);
}

#endif // FILL_H_
