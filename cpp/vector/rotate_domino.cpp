// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm rotateDomino.cpp -o rotateDomino
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
    int minDominoRotations(std::vector<int> &A, std::vector<int> &B) {
        auto size = (int)A.size();
        std::array<int, 7> countA{0}, countB{0};

        for (auto &a : A)
            countA[a]++;

        for (auto &b : B)
            countB[b]++;

        std::cout << "countA = ";
        print(countA);
        std::cout << " countB = ";
        print(countB);
        std::cout << std::endl;

        for (auto i=1; i < 7; ++i) {
            auto sum = countA[i] + countB[i];
            if (sum < size)
                continue;
            if (sum > size)
                sum = size - (sum - size);
            std::cout << "i=" << i;
            std::cout << " countA[" << i << "]=" << countA[i];
            std::cout << " countB[" << i << "]=" << countB[i];
            std::cout << " sum=" << sum << " size=" << size << std::endl;
            if (sum == 0)
                return 0;
            auto flipA = size - countA[i];
            std::cout << "flipA=" << flipA << std::endl;
            auto flipB = size - countB[i];
            std::cout << "flipB=" << flipB << std::endl;
            if (countA[i] > countB[i]) {
                if (flipA <= countB[i])
                    return flipA;
            } else {
                if (flipB <= countA[i])
                    return flipB;
            }
        }

        return -1;        
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
};

void example(const std::vector<int> &argA, const std::vector<int> &argB, int expect) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    auto A(argA);
    auto B(argB);
    std::cout << "A = ";
    solution.print(A);
    std::cout << " B = ";
    solution.print(B);
    std::cout << std::endl;
    auto result = solution.minDominoRotations(A, B);
    std::cout << "result = " << result << std::endl;
    if (result != expect)
        std::cout << "expect = " << expect << std::endl;
}

void f1(void) { example({2,1,2,4,2,2}, {5,2,6,2,3,2}, 2); }
void f2(void) { example({3,5,1,2,3}, {3,6,3,3,4}, -1); }
void f3(void) { example({1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, 0); }
void f4(void) { example({1,2,3,4,6}, {6,6,6,6,5}, 1); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4
};

int main() {
    for (auto &f : examples)
        f();
}
