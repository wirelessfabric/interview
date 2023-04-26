// SPDX-License-Identifier: MIT

#ifndef _UTIL_H_
#define _UTIL_H_

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

template <typename T>
static float sum(const T* v, int n) {
    assert(v && n > 0);
    auto sum{ 0.f };
    do sum += *v++; while (--n);
    return sum;
}

template <typename T>
static float mean(const T* v, int n) {
    return n ? sum(v, n) / (float)n : 0.f;
}

#endif // _UTIL_H_
