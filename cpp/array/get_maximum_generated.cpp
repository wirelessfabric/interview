// SPDX-License-Identifier: MIT
// g++ -std=c++23 -O3 -I.. -lstdc++ -lm get_maximum_generated.cpp -o get_maximum_generated
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. get_maximum_generated.cpp
// cl version 19.35.32215 for x64

#include "common.h"
#include "print.h"
#include "speedup.h"

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

static void example(int n) {
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

static void f1(void) { example(7); }
static void f2(void) { example(2); }
static void f3(void) { example(3); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (const auto& f : examples)
        f();
}
