// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -I.. -lstdc++ -lm can_reach.cpp -o can_reach
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    bool canReach(std::vector<int>& arr, const int start, const int size) {
        const auto val = arr[start];
        if (val < 0)
            return false;
        if (val == 0)
            return true;
        arr[start] = -val;
        const auto right = start + val;
        if (right < size && canReach(arr, right, size) == true)
            return true;
        const auto left = start - val;
        if (left >= 0 && canReach(arr, left, size) == true)
            return true;
        return false;
    }

    bool canReach(std::vector<int>& arr, int start) {
        const auto size = (int)arr.size();
        return canReach(arr, start, size);
    }
};

void example(const std::vector<int> &arg, int start, bool expect) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto arr(arg);
    print(arr, "arr");
    std::cout << "start=" << start << std::endl;
    Solution solution;
    auto result = solution.canReach(arr, start);
    std::cout << "result = " << result << std::endl;
    if (result != expect)
        std::cout << "expect = " << expect << std::endl;
}

void f1(void) { example({4,2,3,0,3,1,2}, 5, true); }
void f2(void) { example({4,2,3,0,3,1,2}, 0, true); }
void f3(void) { example({3,0,2,1,2}, 2, false); }

std::vector<void(*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (auto &f : examples)
        f();
}
