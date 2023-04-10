// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. -lstdc++ -lm house_robber.cpp -o house_robber
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int visitmax(std::vector<int> &v, int &size) {
        auto k = -1;
        auto max = -1;
        for (auto i=0; i < size; ++i) {
            if (v[i] > max) {
                k = i;
                max = v[i];
            }
        }
        if (k != -1) {
            v[k] = -1;
            if (k == 0)
                v[size-1] = -1;
            else
                v[k-1] = -1;
            if (k == size-1)
                v[0] = -1;
            else
                v[k+1] = -1;
        }
        return max;
    }

    int robVisitMax(std::vector<int> &nums) {
        auto v{nums};
        auto size = (int)v.size();
        auto sum = 0, max = 0;
        while ((max = visitmax(v, size)) != -1)
            sum += max;
        return sum;
    }

    int robEveryOther(std::vector<int> &nums) {
        auto size = (int)nums.size();
        auto n = size / 2;
        auto max = 0;
        for (auto i=0; i < size; ++i) {
            auto sum = nums[i];
            auto j = i;
            for (auto k=1; k < n; ++k) {
                j += 2;
                j %= size;
                sum += nums[j];
            }
            if (sum > max)
                max = sum;
        }
        return max;
    }

    int rob(std::vector<int> &nums) {
        auto sumEveryOther = robEveryOther(nums);
        auto sumVisitMax = robVisitMax(nums);
        return sumEveryOther > sumVisitMax ? sumEveryOther : sumVisitMax;
    }
};

void rob(const std::vector<int> &arg, int expect) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    auto nums{arg};
    std::cout << "nums = ";
    print(nums);
    std::cout << std::endl;
    auto result = solution.rob(nums);
    std::cout << "result " << result;
    if (expect != -1 && result != expect)
        std::cout << " expect " << expect;
    std::cout << std::endl;
}

void f1(void) { rob({1,2,3,1}, 4); }
void f2(void) { rob({2,7,9,3,1}, 11); }
void f3(void) { rob({2,1,1,2}, 3); }
void f4(void) { rob({0,0,0}, 0); }
void f5(void) { rob({1,2,2,1}, 3); }
void f6(void) { rob({4,4,0,0}, 4); }
void f7(void) { rob({1,3,1,3,100}, 103); }
void f8(void) { rob({2,3,2}, 3); }
void f9(void) { rob({4,1,2,7,5,3,1}, 14); }
void f10(void) { rob({1,2,3,4,5,1,2,3,4,5}, 16); }
void f11(void) { rob({1,7,9,4}, 11); }
void f12(void) { rob({94,40,49,65,21,21,106,80,92,81,679,4,61,6,237,12,72,74,29,95,265,35,47,1,61,397,52,72,37,51,1,81,45,435,7,36,57,86,81,72}, 2926); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12
};

int main() {
    for (const auto& f : examples)
        f();
}
