// SPDX-License-Identifier: MIT
// g++ -std=c++20 -O3 -I.. -lstdc++ -lm max_profit.cpp -o max_profit
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int maxProfitLeetCodeFastest(std::vector<int> &prices) {
        int minp = INT_MAX, maxp = 0;
        for (int i=0; i < prices.size(); i++) {
            if (minp > prices[i])
                minp = prices[i];
            if (prices[i] - minp > maxp)
                maxp = prices[i] - minp;
        }
        return maxp;
    }

    int maxProfitInRange(std::vector<int> &prices, int buy, size_t size, int max) {
        auto count = size - buy;
        if (count < 2)
            return max;

        auto sell = buy + 1;
        for (; sell < size; ++buy, ++sell)
            if (prices[buy] < prices[sell])
                break;
        if (sell == size)
            return max;

        auto next = 0;
        for (auto j=sell+1; j < size; ++j) {
            if (prices[j] >= prices[sell]) {
                sell = j;
                for (auto i=buy+1; i < sell; ++i)
                    if (prices[i] <= prices[buy])
                        buy = i;
            }
            if (prices[j] < prices[buy] && !next)
                next = j;
        }

        auto profit = prices[sell] - prices[buy];
        if (!next)
            return profit;

        next = maxProfitInRange(prices, next, size, profit);
        return next > profit ? next : profit;
    }

    int maxProfit(std::vector<int> &prices) {
        return maxProfitInRange(prices, 0, prices.size(), 0);
    }
};

static void example(std::vector<int> &nums) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    print(nums, "nums");
    auto result = solution.maxProfit(nums);
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

void example12(void) { 
    std::vector<int> nums{7,1,5,3,6,4};
    example(nums);
}

void example13(void) {
    std::vector<int> nums{7,6,4,3,1};
    example(nums);
}

void example14(void) {
    std::vector<int> nums{2,1,2,1,0,1,2};
    example(nums);
}

void example15(void) {
    std::vector<int> nums{3,3,5,0,0,3,1,4};
    example(nums);
}

void example16(void) {
    std::vector<int> nums{3,2,6,5,0,3};
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
    example12();
    example13();
    example14();
    example15();
    example16();
}
