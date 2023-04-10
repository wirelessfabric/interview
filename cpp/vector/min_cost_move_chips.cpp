// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -lstdc++ -lm minCostMoveChips.cpp -o minCostMoveChips
// g++ --version 9.3.0

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

static const auto speedup = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

class Solution {
public:
    int minCostToMoveChips(std::vector<int> &position) {
        auto even = 0;
        auto odd = 0;
        for (auto &p : position) {
            if (p & 1)
                odd++;
            else
                even++;
        }
        if (even == odd)
            return odd;

        return std::min(odd, even);
    }
};

template<typename T> 
void print(const std::vector<T> &v, int start, int end) {
    std::cout << "[";
    int n = 0;
    for (auto i=start; i <= end; ++i) {
        if (n++)
            std::cout << ",";
        std::cout << v[i];
    }
    std::cout << "]";
}

template<typename T>
void print(const std::array<T, 7> &v) {
    std::cout << "[";
    int n = 0;
    for (const auto &e : v) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    std::cout << "]";
}

template<typename T>
void print(const std::vector<T> &v) {
    std::cout << "[";
    int n = 0;
    for (const auto &e : v) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    std::cout << "]";
}

template<typename T> 
void print(const std::vector<std::vector<T>> &vv) {
    std::cout << "[";
    int n = 0;
    for (const auto &v : vv) {
        if (n++)
            std::cout << ",";
        print(v);
    }
    std::cout << "]";
}

template<typename K, typename V>
void print(const std::map<K,V> &m) {
    std::cout << "[";
    int n = 0;
    for (const auto &it : m) {
        if (n++)
            std::cout << ",";
        std::cout << "[" << it.first << "," << it.second << "]";
    }
    std::cout << "]";
}

void example(const std::vector<int> &arg, int expect) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    auto position(arg);
    std::cout << "position = ";
    print(position);
    std::cout << std::endl;
    auto result = solution.minCostToMoveChips(position);
    std::cout << "result = " << result << std::endl;
    if (result != expect)
        std::cout << "expect = " << expect << std::endl;
}

void f1(void) { example({1,2,3}, 1); }
void f2(void) { example({2,2,2,3,3}, 2); }
void f3(void) { example({1, 1000000000}, 1); }

std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (auto &f : examples)
        f();
}
