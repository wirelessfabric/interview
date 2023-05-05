// SPDX-License-Identifier: MIT
//
// g++ -std=c++23 -O3 -lstdc++ -lm factorial.cpp -o factorial
// g++ --version 11.3.0 on soho ubuntu 22.04

#include <cmath>
#include <vector>
#include <array>
#include <map>
#include <utility>
#include <functional>
#include <iostream>

template<long n> struct Factorial { static const long value = n * Factorial<n-1>::value; };
template<> struct Factorial<0> { static const long value = 1; };

void factorial_constexpr_templated_recursion(void) {
    std::cout << "factorial_constexpr_templated_recursion " << std::endl;
    constexpr long n = 20;
    auto result = Factorial<n>::value;
    std::cout << "Factorial(" << n << ") = " << result << std::endl;
}

constexpr long constexpr_recursive_function(long n) {
    return n > 0 ? n * constexpr_recursive_function(n - 1) : 1;
}

long recursive_function(long n) {
    return n > 0 ? n * recursive_function(n - 1) : 1;
}

long std_function_lambda(long n) {
    std::function<long(long)> f = [&](long n) {
        return n > 0 ? n * f(n - 1) : 1;
    };
    return f(n);
}

long explicit_self_parameter_lambda(long n) {
#if 0 // compile errors
    auto f = [](long n, auto&& self) {
        return n > 0 ? n * self(n - 1, self) : 1;
    };
    return f(n, f);
#else
    return recursive_function(n);
#endif
}

long cpp23_deducing_lambda(long n) {
#if 0 // requires c++23 for c++ deducing
    long f = [](this auto&& self, long n) {
        return n > 0 ? n * self(n - 1) : 1;
    };
    return f(n);
#else
    return recursive_function(n);
#endif
}

void factorial(long n, long (*f)(long)) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto result = f(n);
    std::cout << "factorial(" << n << ") = " << result << std::endl;
}

void f0(void) { factorial(20, cpp23_deducing_lambda); }
static void f1(void) { factorial(20, explicit_self_parameter_lambda); }
static void f2(void) { factorial(20, std_function_lambda); }
static void f3(void) { factorial(20, recursive_function); }
static void f4(void) { factorial(12, recursive_function); }
static void f5(void) { factorial(5,  recursive_function); }
static void f6(void) { factorial(20, constexpr_recursive_function); }
static void f7(void) { factorial(12, constexpr_recursive_function); }
static void f8(void) { factorial(5,  constexpr_recursive_function); }
static void f9(void) { factorial_constexpr_templated_recursion(); }

static std::vector<void (*)(void)> examples {
    f0, f1, f2, f3, f4, f5, f6, f7, f8, f9
};

int main() {
    for (const auto& f : examples)
        f();
}
