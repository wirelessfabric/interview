// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. average.cpp -o average
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. average.cpp
// cl version 19.35.32215 for x64

// https://www.youtube.com/watch?v=rUt9xcPyKEY
// Lightning Talk: Finding the Average of 2 Integers - Tomer Vromen - CppCon 2022

#include "common.h"
#include "print.h"
#include "speedup.h"

// https://www.youtube.com/watch?v=rUt9xcPyKEY @ 1:00

double average(int a, int b) {
    return (1.0 * a + b) / 2.0;
}

// https://www.youtube.com/watch?v=rUt9xcPyKEY @ 1:20

template<typename T>
requires std::is_integral_v<T>
double average(T a, T b) {
    return (1.0 * a + b) / 2.0;
}

template<typename T>
requires std::is_integral_v<T>
void example(T a, T b, double target) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << ": ";
    std::cout << "a = " << a << " b = " << b << " target = " << target;

    auto result = average(a, b);
    std::cout << " result = " << result;
    if (result == target)
        std::cout << " pass" << std::endl;
    else
        std::cout << " fail" << std::endl;
}

void f1(void) { example(1, 2, 1.5); }
void f2(void) { example(100, 23, 61.5); }
void f3(void) { long long n = (1LL << 53) + 1; example(n, n, 9007199254740993); }

std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (const auto& f : examples)
        f();
}
