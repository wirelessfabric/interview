// SPDX-License-Identifier: MIT

#ifndef UTILS_H_
#define UTILS_H_

#include <map>
#include <vector>
#include <random>

template <typename T>
static void flip(T *v, int n) {
    assert(v && n > 0);
    auto tail{ &v[n - 1] };
    while (v < tail) {
        auto t{ *v };
        *v++ = *tail;
        *tail-- = t;
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

template <typename T>
static void histogram(const std::vector<T>& v, std::map<T,int>& map) {
    for (const auto& key : v)
        map[key]++;
}

static std::mt19937& prng(void) {
    static std::mt19937 g(std::random_device{}());
    return g;
}

#endif // UTILS_H_
