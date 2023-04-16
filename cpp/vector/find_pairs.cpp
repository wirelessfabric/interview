// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. -lstdc++ -lm find_pairs.cpp -o find_pairs
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int findPairs(std::vector<int> &nums, int k) {
        std::unordered_map<int, int> map;
        for (auto &v : nums)
            map[v]++;

        auto count = 0;
        for (const auto &it : map)
            if ((k && map.find(it.first + k) != map.end()) || (!k && it.second > 1))
                count++;
        return count;
    }
};

static void example(const std::vector<int> &nums, int k) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "nums = ";
    print(nums);
    std::cout << ", k = " << k << std::endl;

    std::vector<int> arg(nums);
    auto result = solution.findPairs(arg, k);
    std::cout << "result = " << result << std::endl;
}

static void f1(void) { example({3,1,4,1,5}, 2); }
static void f2(void) { example({1,2,3,4,5}, 1); }
static void f3(void) { example({1,3,1,5,4}, 0); }
static void f4(void) { example({1,2,4,4,3,3,0,9,2,3}, 3); }
static void f5(void) { example({-1,-2,-3}, 1); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5
};

int main() {
    for (const auto& f : examples)
        f();
}
