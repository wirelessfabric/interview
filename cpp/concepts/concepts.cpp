// SPDX-License-Identifier: MIT
// Hendrik Niemeyer https://www.youtube.com/watch?v=N_kPd2OK1L8
// An Introduction to C++20’s Concepts [ C++ on Sea 2020 ]

// g++ -std=c++23 -O3 -lstdc++ -lm concepts.cpp -o concepts
// g++ --version 11.3.0 on soho ubuntu 22.04

#include <stdio.h>

#include <concepts>

struct X {};

template <typename Callable, typename Arg>
void print(Callable callable, Arg arg) {
  puts("Base case without constraints!");
}

template <typename Callable, typename Arg>
requires std::predicate<Callable, Arg>
void print(Callable callable, Arg arg) {
  puts("This is a predicate!");
}

template <typename Callable, typename Arg>
requires std::invocable<Callable, Arg>
void print(Callable callable, Arg arg) {
  puts("This is a general callable!");
}

int main() {
  const auto predicate = [](int a) { return (a > 10); };
  const auto regular_function = [](double a) { return X(); };
  print(predicate, 5);
  print(regular_function, 10.0);
  print(regular_function, X());
}
