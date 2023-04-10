// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm productLessThanK.cpp -o productLessThanK
// g++ --version 9.3.0

#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <climits>

class Solution {
public:
    int numSubarrayProductLessThanK(std::vector<int> &nums, int k) {
        if (k < 2)
            return 0;
        auto count = 0;
        for (auto i=0; i < nums.size(); ++i) {
            auto product = nums[i];
            if (product == 1)
                count += 2;
            else if (product < k)
                count++;
            else
                break;
            for (auto j=i+1; j < nums.size(); ++j) {
                if (nums[j] == 1) {
                    i = j;
                    count += 2;
                    continue;
                }
                product *= nums[j];
                if (product < k)
                    count++;
                else
                    break;
            }
        }
        return count;
    }

    void print(const std::vector<int> &v, int start, int end) {
        std::cout << "[";
        int n = 0;
        for (auto i=start; i <= end; ++i) {
            if (n++)
                std::cout << ",";
            std::cout << v[i];
        }
        std::cout << "]";
    }

    void print(const std::vector<int> &v) {
        std::cout << "[";
        int n = 0;
        for (const auto &e : v) {
            if (n++)
                std::cout << ",";
            std::cout << e;
        }
        std::cout << "]";
    }

    void print(const std::vector<std::vector<int>> &vv) {
        std::cout << "[";
        int n = 0;
        for (const auto &v : vv) {
            if (n++)
                std::cout << ",";
            print(v);
        }
        std::cout << "]";
    }
};

void example(std::vector<int> &nums, int k) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "nums = ";
    solution.print(nums);
    std::cout << " k=" << k << std::endl;
    auto result = solution.numSubarrayProductLessThanK(nums, k);
    std::cout << "result " << result << std::endl;
}

void example1(void) {
    std::vector<int> nums{1,2,3,1};
    example(nums, 100);
}

void example2(void) {
    std::vector<int> nums{2,7,9,3,1};
    example(nums, 100);
}

void example3(void) {
    std::vector<int> nums{2,1,1,2};
    example(nums, 100);
}

void example4(void) {
    std::vector<int> nums{0,0,0};
    example(nums, 100);
}

void example5(void) {
    std::vector<int> nums{1,2,2,1};
    example(nums, 100);
}

void example6(void) {
    std::vector<int> nums{4,4,0,0};
    example(nums, 100);
}

void example7(void) {
    std::vector<int> nums{1,3,1,3,100};
    example(nums, 100);
}

void example8(void) {
    std::vector<int> nums{3,10,5,25,2,8};
    example(nums, 100);
}

void example9(void) {
    std::vector<int> nums{8,10,2};
    example(nums, 100);
}

void example10(void) {
    std::vector<int> nums{1};
    example(nums, 100);
}

void example11(void) {
    std::vector<int> nums{4,6,7};
    example(nums, 100);
}

void example12(void) { 
    std::vector<int> nums{7,1,5,3,6,4};
    example(nums, 100);
}

void example13(void) {
    std::vector<int> nums{7,6,4,3,1};
    example(nums, 100);
}

void example14(void) {
    std::vector<int> nums{2,1,2,1,0,1,2};
    example(nums, 100);
}

void example15(void) {
    std::vector<int> nums{3,3,5,0,0,3,1,4};
    example(nums, 100);
}

void example16(void) {
    std::vector<int> nums{3,2,6,5,0,3};
    example(nums, 100);
}

void example17(void) {
    std::vector<int> nums{10,5,2,6};
    example(nums, 100);
}

void example18(void) {
    std::vector<int> nums{1,1,1};
    example(nums, 2);
}

void example19(void) {
    std::vector<int> nums{1,2,3};
    example(nums, 0);
}

void example20(void) {
    std::vector<int> nums{1,1,1};
    example(nums, 1);
}

int main(int argc, char **argv) {
    example17();
    example18();
    example19();
    example20();
}
