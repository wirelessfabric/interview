// SPDX-License-Identifier: MIT

#ifndef _FILL_H_
#define _FILL_H_

#include <numbers>

template <typename T>
T slice(int n) {
    constexpr auto pie{ (T)(2.0 * std::numbers::pi) };
    return pie / static_cast<T>(n);
}

static void fill(float* v, float (*f)(float), int n) {
    assert(v && f && n > 0);
    auto w{ 0.f };
    const auto df{ slice<float>(n) };
    do { *v++ = f(w); w += df; } while (--n);
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

static void fill_random(float* v, float min, float max, int n) {
    std::mt19937 g(std::random_device{}());
    std::uniform_real_distribution<float> d(min, max);
    do *v++ = d(g); while (--n);
}

#endif // _FILL_H_
