// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. bit_width.cpp -o bit_width
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. bit_width.cpp
// cl version 19.35.32215 for x64

// https://www.youtube.com/watch?v=rUt9xcPyKEY
// Lightning Talk: Finding the Average of 2 Integers - Tomer Vromen - CppCon 2022

#include "common.h"
#include "print.h"
#include "speedup.h"
#include <cmath>

// https://www.youtube.com/watch?v=rUt9xcPyKEY @ 4:03

size_t bit_width(size_t n) {
    return n ? log2(n) + 1 : 1;
}

// https://www.youtube.com/watch?v=rUt9xcPyKEY @ 4:28

size_t decimal_width(size_t n) {
    return n ? log10(n) + 1 : 1;
}

void example(size_t (*f)(size_t), size_t n, size_t target) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << ": ";
    std::cout << "n = " << n << " target = " << target;

    auto result = f(n);
    std::cout << " result = " << result;
    if (result == target)
        std::cout << " pass" << std::endl;
    else
        std::cout << " fail" << std::endl;
}

void f1(void) { example(bit_width, 0, 1); }
void f2(void) { example(bit_width, 5, 3); }
void f3(void) { example(bit_width, 20, 5); }
void f4(void) { example(decimal_width, 0, 1); }
void f5(void) { example(decimal_width, 5, 1); }
void f6(void) { example(decimal_width, 20, 2); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6
};

int main() {
    for (auto &f : examples)
        f();
}