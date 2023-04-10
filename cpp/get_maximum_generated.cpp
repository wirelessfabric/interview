// SPDX-License-Identifier: MIT
// g++ -std=c++23 -O3 -lstdc++ -lm get_maximum_generated.cpp -o get_maximum_generated
// g++ --version 11.3.0 on soho ubuntu 22.04

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <climits>

void print_begin(const char *text = nullptr) {
    if (text)
        std::cout << text << " = ";
    std::cout << "[";
}

void print_end(const char *text = nullptr) {
    std::cout << "]";
    if (text)
        std::cout << std::endl;
}

template<typename T> 
void print(const std::vector<T> &v, int start, int end, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (auto i=start; i <= end; ++i) {
        if (n++)
            std::cout << ",";
        std::cout << v[i];
    }
    print_end(text);
}

template<typename T, size_t N>
void print(const std::array<T, N> &a, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &e : a) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template<typename T>
void print(const std::vector<T> &v, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &e : v) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template<typename T> 
void print(const std::vector<std::vector<T>> &vv, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &v : vv) {
        if (n++)
            std::cout << ",";
        print(v);
    }
    print_end(text);
}

template<typename K, typename V>
void print(const std::map<K,V> &m, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &it : m) {
        if (n++)
            std::cout << ",";
        std::cout << "[" << it.first << "," << it.second << "]";
    }
    print_end(text);
}

static const auto speedup = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

class Solution {
public:
    int getMaximumGenerated(int n) {
        std::array<int, 200> nums;
        nums[0] = 0;
        nums[1] = 1;
        for (auto i=2; i <= n; ++i) {
            std::cout << "loop = " << i << std::endl;
        }
        return 0;
    }
};

void example(int n) {
    static auto counter = 1;
    std::cout << "getMaximumGenerated" << counter++ << std::endl;
    std::cout << "n = " << n << std::endl;
    if (n < 0 || n > 100) {
        std::cout << "constraints: 0 <= n <= 100" << std::endl;
        return;
    }
    Solution solution;
    auto result = solution.getMaximumGenerated(n);
    std::cout << "result = " << result << std::endl;
}

void f1(void) { example(7); }
void f2(void) { example(2); }
void f3(void) { example(3); }

std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (auto &f : examples)
        f();
}
