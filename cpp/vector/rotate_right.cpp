// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -I.. rotate_right.cpp -o rotate_right -lm -ltbb
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++17 /I.. rotate_right.cpp
// cl version 19.35.32215 for x64

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    void rotateRightByOne(std::vector<int> &nums, int j) {
        auto t = nums[j];
        for (; j; --j)
            nums[j] = nums[j-1];
        nums[0] = t;
    }

    void rotate(std::vector<int> &nums, int k) {
        auto size = (int)nums.size();
        if (size < 2)
            return;
        if (k < 1)
            return;

        if (k > size)
            k %= size;
        
        if (k == 1) {
            rotateRightByOne(nums, size-1);
            return;
        }

        auto x = size - k;

        auto i=0;
        auto j=k;
        std::cout << "j=k=" << j << " i=" << i << " x=" << x << std::endl;
        auto t=nums[j+k];
        for (; j < x;) {
            nums[j+k] = nums[j]; nums[j++] = nums[i++];
        }
        for (j=0; i < k; i++, j++, x++) {
            nums[j] = nums[x]; nums[x] = nums[i];
        }
        nums[j] = t;
     }

};

void rotateRight(const std::vector<int> &arg, int k) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    auto nums(arg);
    std::cout << "nums = ";
    print(nums);
    std::cout << ", k = " << k << std::endl;
    solution.rotate(nums, k);
    std::cout << "result = ";
    print(nums);
    std::cout << std::endl;
}

void f1(void) { rotateRight({-1,0,3,5,9,12}, 9); }
void f2(void) { rotateRight({-1,0,3,5,9,12}, 2); }
void f3(void) { rotateRight({-1,0,3,5,9,12}, 1); }
void f4(void) { rotateRight({-1,0,3,5,9,12}, 0); }
void f5(void) { rotateRight({-1,0,3,5,9,12}, 3); }
void f6(void) { rotateRight({-1,0,3,5,9,12}, 5); }
void f7(void) { rotateRight({-1,0,3,5,9,12}, 12); }
void f8(void) { rotateRight({5}, 5); }
void f9(void) { rotateRight({1,2,3,4,5,6,7}, 3); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8, f9
};

int main() {
    for (const auto& f : examples)
        f();
}
