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
#include "utils.h"
#include "speedup.h"

static std::mt19937 g(std::random_device{}());

/////////////////////////////////////////////////////////////////////

template <typename T>
requires std::is_floating_point_v<T>
static T random_normal_distribution(void) {
    static std::normal_distribution<T> d;
    return d(g);
}

template <typename T>
static void random_normal_distribution(std::vector<T>& v) {
    for (auto& data : v)
        data = random_normal_distribution<T>();
}

/////////////////////////////////////////////////////////////////////

template <typename T>
requires std::is_integral_v<T>
static T random_uniform_distribution(void) {
    static std::uniform_int_distribution<T> d;
    return d(g);
}

template <typename T>
requires std::is_floating_point_v<T>
static T random_uniform_distribution(void) {
    static std::uniform_real_distribution<T> d;
    return d(g);
}

template <typename T>
static void random_uniform_distribution(std::vector<T>& v) {
    for (auto& data : v)
        data = random_uniform_distribution<T>();
}

/////////////////////////////////////////////////////////////////////
// ISSUE: % operation makes lower numbers slightly more likely
// https://www.youtube.com/watch?v=6DPkyvkMkk8 @ 487:40

template <typename T>
static void dice_roll_std_rand(std::vector<T>& v) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    std::srand(ts.tv_nsec);
    for (auto& data : v)
        data = std::rand() % 6u + 1u;
}

template <typename T>
static T dice_roll_std_mt19937(void) {
    static std::uniform_int_distribution<int> d(1, 6);
    return (T)d(g);
}

template <typename T>
static void dice_roll_std_mt19937(std::vector<T>& v) {
    for (auto& data : v)
        data = dice_roll_std_mt19937<T>();
}

/////////////////////////////////////////////////////////////////////

static auto counter = 1;

template <typename T>
static void example(void (*f)(std::vector<T>&), int n) {
    std::cout << "Example " << counter++ << ": " << std::endl;
    std::cout << "n = " << n;

    std::vector<T> v(n, (T)0);
    f(v);
    print(v, ", v");

    std::map<T, int> h{};
    histogram(v, h);
    print(h, "histogram");
}

static void f1(void) { example<float>(random_normal_distribution, 10); }
static void f2(void) { example<float>(random_uniform_distribution, 10); }
static void f3(void) { example<int>(random_uniform_distribution, 10); }
static void f4(void) { example<float>(dice_roll_std_rand, 50); }
static void f5(void) { example<int>(dice_roll_std_rand, 50); }
static void f6(void) { example<float>(dice_roll_std_mt19937, 50); }
static void f7(void) { example<int>(dice_roll_std_mt19937, 50); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7
};

int main() {
    for (const auto& f : examples)
        f();
}
