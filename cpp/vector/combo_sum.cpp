// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. -lstdc++ -lm combo_sum.cpp -o combo_sum
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    void combinationSum3(int k, int n, std::vector<int> combo, int base, std::vector<std::vector<int>> &result) {
        if (n == 0 && combo.size() == k) {
            std::vector<int> copy(combo);
            result.push_back(copy);
            return;
        }
        else if (n < 0 || combo.size() == k)
            return;

        for (auto i=base; i < 9; ++i) {
            combo.push_back(i + 1);
            combinationSum3(k, n - i - 1, combo, i + 1, result);
            combo.pop_back();
        }
    }

    std::vector<std::vector<int>> combinationSum3(int k, int n) {
        std::vector<std::vector<int>> result;
        std::vector<int> combo;
        combinationSum3(k, n, combo, 0, result);
        return result;
    }

    void combinationSum(std::vector<int> candidates, int target, std::vector<int> combo, int sum, int base, std::vector<std::vector<int>> &result) {
        for (auto i=base; i < candidates.size(); ++i) {
            auto num = candidates[i];
            while (true) {
                combinationSum(candidates, target, combo, sum, i + 1, result);
                if (sum + num > target)
                    break;
                sum += num;
                combo.push_back(num);
                if (sum == target) {
                    if (result.size())
                        for (auto it=result.rbegin(); it != result.rend(); it++)
                            if (combo == *it)
                                return;
                    result.push_back(combo);
                }
            }
        }
    }

    std::vector<std::vector<int>> combinationSum(std::vector<int> candidates, int target) {
        std::vector<std::vector<int>> result;
        std::vector<int> combo;
        combinationSum(candidates, target, combo, 0, 0, result);
        return result;
    }
};

void example(const std::vector<int> &candidates, int target) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "candidates = ";
    print(candidates);
    std::cout << ", target = " << target << std::endl;

    auto result = solution.combinationSum(candidates, target);
    std::cout << "result = ";
    print(result);
    std::cout << std::endl;
}

void f1(void) { example({2, 3, 6, 7}, 7); }
void f2(void) { example({2, 3, 5}, 8); }
void f3(void) { example({2}, 1); }
void f4(void) { example({1}, 1); }
void f5(void) { example({1}, 2); }
void f6(void) { example({8,7,4,3}, 11); }
void f7(void) { example({7,3,9,6}, 6); }
void f8(void) { example({7,5,8,12,3,10,9,4,11,6}, 21); }
void f9(void) { example({5,4,2,3}, 14); }
void f10(void) { example({48,22,49,24,26,47,33,40,37,39,31,46,36,43,45,34,28,20,29,25,41,32,23}, 69); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10
};

int main() {
    for (auto &f : examples)
        f();
}
