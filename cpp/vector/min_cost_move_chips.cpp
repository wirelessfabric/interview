// SPDX-License-Identifier: MIT
//
// g++ -std=c++17 -O3 -I.. min_cost_move_chips.cpp -o min_cost_move_chips
// g++ --version 11.3.0 on soho x86_64 ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

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

static void example(const std::vector<int> &arg, int expect) {
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

static void f1(void) { example({1,2,3}, 1); }
static void f2(void) { example({2,2,2,3,3}, 2); }
static void f3(void) { example({1, 1000000000}, 1); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (const auto& f : examples)
        f();
}
