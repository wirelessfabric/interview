// SPDX-License-Identifier: MIT

#ifndef _FILL_H_
#define _FILL_H_

#include <numbers>

template <typename T>
T slice(int n) {
    constexpr T pie = 2.0 * std::numbers::pi;
    return pie / static_cast<T>(n);
}

static void fill(float* v, int n, float (*f)(float)) {
    assert(v && f && n > 0);
    auto w{ 0.f };
    const auto df{ slice<float>(n) };
    do { *v++ = f(w); w += df; } while (--n);
}

static void fill_gaussian(float* v, float f, int n) {
    assert(v && f > 0 && n > 0);
    const auto tau{ (float)n / ((float)(2.0 * std::numbers::pi) * f) };
    const auto dt{ 1.f / f };
    auto t{ -dt * (float)(n - 1) * 0.5f };
    do {
        const auto k{ t / tau };
        *v++ = expf(-k * k);
        t += dt;
    } while (--n);
}

template <typename T>
static void reverse(T *v, int n) {
    assert(v && n > 0);
    auto tail{ &v[n - 1] };
    while (v < tail) {
        auto f{ *v };
        *v++ = *tail;
        *tail-- = f;
    }
}

#endif // _FILL_H_
