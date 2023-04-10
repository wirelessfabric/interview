// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm bitwise_complement.cpp -o bitwise_complement
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

class Solution {
public:
    int bitwiseComplement(int n) {
        auto x = 0;
        auto bit = 1;
        while (n) {
            if ((n & 1) == 0)
                x |= bit;
            bit <<= 1;
            n >>= 1;
        }
        return x;
    }

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
};

void example(int N) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "N = " << N << std::endl;
    auto result = solution.bitwiseComplement(N);
    std::cout << "result = " << result << std::endl;
}

void f1(void) { example(5); }
void f2(void) { example(7); }
void f3(void) { example(10); }

std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (auto &f : examples)
        f();
}
