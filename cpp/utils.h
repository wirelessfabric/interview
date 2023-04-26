// SPDX-License-Identifier: MIT

#ifndef _UTILS_H_
#define _UTILS_H_

template <typename T>
static void reverse(T *v, int n) {
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

#endif // _UTILS_H_
