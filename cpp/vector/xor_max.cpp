// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm xor_max.cpp -o xor_max
// g++ --version 9.3.0

#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>

struct TrieNode {
    TrieNode *child[2];
    TrieNode() {}
    ~TrieNode() {
        if (child[0]) delete child[0];
        if (child[1]) delete child[1];
    }
};

class Solution {
public:
    int findMaximumXORNaive(std::vector<int> &nums) {
        auto size = nums.size();
        auto max = 0;
        for (auto i=0; i < size; ++i) {
            auto n = nums[i];
            for (auto j=i+1; j < size; ++j) {
                auto x = n ^ nums[j];
                if (x > max) {
                    max = x;
                }
            }
        }
        return max;        
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

static void example(std::vector<int> &nums) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "nums = ";
    solution.print(nums);
    std::cout << std::endl;
    auto result = solution.findMaximumXORNaive(nums);
    std::cout << "result " << result << std::endl;
}

void example1(void) {
    std::vector<int> nums{1,2,3,1};
    example(nums);
}

void example2(void) {
    std::vector<int> nums{2,7,9,3,1};
    example(nums);
}

void example3(void) {
    std::vector<int> nums{2,1,1,2};
    example(nums);
}

void example4(void) {
    std::vector<int> nums{0,0,0};
    example(nums);
}

void example5(void) {
    std::vector<int> nums{1,2,2,1};
    example(nums);
}

void example6(void) {
    std::vector<int> nums{4,4,0,0};
    example(nums);
}

void example7(void) {
    std::vector<int> nums{1,3,1,3,100};
    example(nums);
}

void example8(void) {
    std::vector<int> nums{3,10,5,25,2,8};
    example(nums);
}

void example9(void) {
    std::vector<int> nums{8,10,2};
    example(nums);
}

void example10(void) {
    std::vector<int> nums{1};
    example(nums);
}

void example11(void) {
    std::vector<int> nums{4,6,7};
    example(nums);
}

int main(int argc, char **argv) {
    example1();
    example2();
    example3();
    example4();
    example5();
    example6();
    example7();
    example8();
    example9();
    example10();
    example11();
}
