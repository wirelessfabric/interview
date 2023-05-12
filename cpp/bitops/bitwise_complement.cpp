// SPDX-License-Identifier: MIT
//
// g++ -std=c++11 -O3 bitwise_complement.cpp -o bitwise_complement
// g++ --version 11.3.0 on soho x86_64 ubuntu 22.04

#include <iostream>
#include <vector>

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
};

static void example(int N) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "N = " << N << std::endl;
    auto result = solution.bitwiseComplement(N);
    std::cout << "result = " << result << std::endl;
}

static void f1(void) { example(5); }
static void f2(void) { example(7); }
static void f3(void) { example(10); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (const auto& f : examples)
        f();
}
