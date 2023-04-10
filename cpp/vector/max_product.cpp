// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. -lstdc++ -lm max_product.cpp -o max_product
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int maxProduct(std::vector<int>& nums) {
        auto size = (int)nums.size();
        auto max = std::numeric_limits<int>::min();
        for (auto i=0; i < size; ++i) {
            auto val = 1;
            for (auto j=i; j < size; ++j) {
                val *= nums[j];
                if (val > max)
                    max = val;
            }
        }
        return max;
    }

    int maxProductLeetCodeFastest(std::vector<int>& nums) {
        int maxProduct=nums[0], maxVal=nums[0], minVal=nums[0];
        for (int i=1; i<nums.size(); i++) {
            if (nums[i]<0)
                std::swap(maxVal, minVal);
            
            maxVal = std::max(maxVal*nums[i], nums[i]);
            minVal = std::min(minVal*nums[i], nums[i]);
            maxProduct = std::max(maxProduct, maxVal);
        }
        return maxProduct;
    }
};

void example(std::vector<int>& nums) {
    Solution solution;
    std::cout << "nums ";
    print(nums);
    std::cout << std::endl;
    auto result = solution.maxProduct(nums);
    std::cout << "result " << result << std::endl;
}

void example1() {
    std::vector<int> nums{4,9,5};
    example(nums);
}

void example2() {
    std::vector<int> nums{9,4,9,8,4};
    example(nums);
}

void example3() {
    std::vector<int> nums{2,3,-2,4};
    example(nums);
}

void example4() {
    std::vector<int> nums{-2,0,-1};
    example(nums);
}

void example5() {
    std::vector<int> nums{-2,3,-4};
    example(nums);
}

int main() {
    example1();
    example2();
    example3();
    example4();
    example5();
}
