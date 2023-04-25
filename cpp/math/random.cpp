// SPDX-License-Identifier: MIT
//
// g++ -std=c++2a -O3 -I.. random.cpp -o random
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// clang++ -std=c++2a -O3 -I.. random.cpp -o random
// Apple clang++ version 14.0.0 (clang-1400.0.29.202) on Apple M1 macOS Ventura 13.2.1
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. random.cpp
// cl version 19.35.32215 for x64

// https://www.youtube.com/watch?v=N2AM6ixC6LI
// Fast, High-quality Pseudo-random Numbers for Audio Developers - Roth Michaels - ADC22

#include "common.h"
#include "print.h"
#include "speedup.h"

// https://www.youtube.com/watch?v=N2AM6ixC6LI @ 3:23
void random_std_mt19937(std::vector<float>& v) {
    std::random_device r;
    std::mt19937 e(r());
    std::uniform_real_distribution<float> d;
    for (auto& data : v)
        data = d(e);
}

static void example(void (*f)(std::vector<float>&), int n) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << ": ";
    std::cout << "n = " << n;

    std::vector<float> v(n, 0.f);
    f(v);
    print(v, ", v");
}

static void f1(void) { example(random_std_mt19937, 10); }

static std::vector<void (*)(void)> examples {
    f1
};

int main() {
    for (const auto& f : examples)
        f();
}
