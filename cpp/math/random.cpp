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

// https://www.youtube.com/watch?v=wDj64pSeQ4I
// Is rand() Actually Random?

// https://www.youtube.com/watch?v=Gb-1grkVGSg
// rand() Considered Harmful : Stephan T. Lavavej

// https://www.youtube.com/watch?v=4_QO1nm7uJs
// I Just Wanted a Random Integer! : Cheinan Marks @ CppCon 2016

// https://www.youtube.com/watch?v=6DPkyvkMkk8
// What C++ Programmers Need to Know about Header ＜random＞ : Walter E. Brown @ CppCon 2016

// https://www.youtube.com/watch?v=Tof5pRedskI
// Real-time Programming with the C++ Standard Library : Timur Doumler @ CppCon 2021

// https://www.youtube.com/watch?v=N2AM6ixC6LI
// Fast, High-quality Pseudo-random Numbers for Audio Developers : Roth Michaels @ ADC22

#include "common.h"
#include "print.h"
#include "speedup.h"

// https://www.youtube.com/watch?v=N2AM6ixC6LI @ 3:23
static void random_std_mt19937(std::vector<float>& v) {
    static std::mt19937 g(std::random_device{}());
    static std::uniform_real_distribution<float> d;
    for (auto& data : v)
        data = d(g);
}

// https://www.youtube.com/watch?v=N2AM6ixC6LI @ 11:12
static void random_std_rand_dice_roll(std::vector<float>& v) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    std::srand(ts.tv_nsec);
    for (auto& data : v)
        data = std::rand() % 6u + 1u;
}

// https://www.youtube.com/watch?v=N2AM6ixC6LI @ 12:42
static void random_std_mt19937_dice_roll(std::vector<float>& v) {
    static std::mt19937 g(std::random_device{}());
    static std::uniform_real_distribution<float> d(1u, 6u);
    for (auto& data : v)
        data = (float)(int)d(g);
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
static void f2(void) { example(random_std_rand_dice_roll, 12); }
static void f3(void) { example(random_std_mt19937_dice_roll, 12); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (const auto& f : examples)
        f();
}
