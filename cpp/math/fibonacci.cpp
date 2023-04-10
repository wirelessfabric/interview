// SPDX-License-Identifier: MIT
// g++ -std=c++23 -O3 -lstdc++ -lm fibonacci.cpp -o fibonacci
// g++ --version 11.3.0 on soho ubuntu 22.04

#include <cmath>
#include <vector>
#include <array>
#include <map>
#include <utility>
#include <iostream>

// C++ Weekly - Ep 13 Fibonacci: You're Doing It Wrong
// https://www.youtube.com/watch?v=hErD6WGqPlA&feature=emb_logo
// Recursion Algorithm @ 5:15

int fib_recursion(int n) {
    if (n == 0) return 0;
    if (n <= 2) return 1;
    return fib_recursion(n-1) + fib_recursion(n-2);
}

// C++ Weekly - Ep 13 Fibonacci: You're Doing It Wrong
// https://www.youtube.com/watch?v=hErD6WGqPlA&feature=emb_logo
// Compile-Time Templated Recursion Algorithm @ 7:09

template<int I> struct Fib { static const int val = Fib<I-1>::val + Fib<I-2>::val; };
template<> struct Fib<0> { static const int val = 0; };
template<> struct Fib<1> { static const int val = 1; };

int fib_constexpr_templated_recursion(void) {
    std::cout << "fib_constexpr_templated_recursion " << std::endl;
    constexpr auto n = 45;
    auto result = Fib<n>::val;
    std::cout << "fib(" << n << ") = " << result << std::endl;
    return result;
}

// C++ Weekly - Ep 13 Fibonacci: You're Doing It Wrong
// https://www.youtube.com/watch?v=hErD6WGqPlA&feature=emb_logo
// Runtime Templated Recursion Algorithm @ 17:44

template<size_t ... I>
int fib_gen_templated_recursion(std::index_sequence<I...>, const int i) {
    constexpr std::array<int, sizeof...(I)> a = { Fib<I>::val... };
    return a[i];
}

int fib_runtime_templated_recursion(const int n) {
    return fib_gen_templated_recursion(std::make_index_sequence<47>(), n);
}

// C++ Weekly - Ep 13 Fibonacci: You're Doing It Wrong
// https://www.youtube.com/watch?v=hErD6WGqPlA&feature=emb_logo
// Binet Algorithm @ 22:15

constexpr int fib_binet(const int n) {
    if (n == 0) return 0;
    if (n <= 2) return 1;
    constexpr auto sqrt5 = std::sqrt(5);
    return static_cast<int>((std::pow(1 + sqrt5, n) - std::pow(1 - sqrt5, n)) / (std::pow(2, n) * sqrt5));
}

// Dynamic Programming I: Fibonacci, Shortest Paths
// https://www.youtube.com/watch?v=OQ5jsbhAv_M
// Memoized Recursion DP Algorithm @ 13:30

int fib_dp_memoized_recursion(int n) {
    if (n == 0) return 0;
    if (n <= 2) return 1;
    static std::map<int, int> fib;
    auto it = fib.find(n);
    if (it != fib.end()) return it->second;
    auto f = fib_dp_memoized_recursion(n-1) + fib_dp_memoized_recursion(n-2);
    fib[n] = f;
    return f;
}

// Dynamic Programming I: Fibonacci, Shortest Paths
// https://www.youtube.com/watch?v=OQ5jsbhAv_M
// Bottom-Up DP Algorithm @ 26:44

int fib_dp_bottom_up(int n) {
    static std::map<int, int> fib;
    for (auto k=0; k <= n; ++k) {
        int f;
        if (k == 0) f = 0;
        else if (k <= 2) f = 1;
        else f = fib[k-1] + fib[k-2];
        fib[k] = f;
    }
    return fib[n];
}

// C++ Weekly - Ep 51 - Advanced Stateful Lambdas
// https://www.youtube.com/watch?v=GeLp4F-Svc8
// Stateful Lambda Algorithm @ 1:08

int fib_stateful_lambda(int n) {
    auto fib_lambda = [i = 0, j = 1]() mutable {
        i = std::exchange(j, j+i);
        return i;
    };

    while (--n)
        fib_lambda();
    
    return fib_lambda();
}

void fib(const int n) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto result = fib_binet(n);
    std::cout << "fib(" << n << ") = " << result << std::endl;
}

void f0(void) { fib_constexpr_templated_recursion(); }
void f1(void) { fib(45); }
void f2(void) { fib(46); }
void f3(void) { fib(47); }

std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (auto &f : examples)
        f();
}
