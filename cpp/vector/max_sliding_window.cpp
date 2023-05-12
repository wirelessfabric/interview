// SPDX-License-Identifier: MIT
//
// g++ -std=c++17 -O3 max_sliding_window.cpp -o max_sliding_window
// g++ --version 11.3.0 on soho x86_64 ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    std::vector<int> maxSlidingWindowLeetCode(std::vector<int>& nums, int k) {         
        std::vector<int> res;
        size_t n = nums.size();
        if (n * k == 0) return res;
        if (k == 1) return nums;
        
        std::vector<int> l(n);
        std::vector<int> r(n);
        l[0] = nums[0];
        r[n - 1] = nums[n - 1];
        for (int i = 1; i < n; i++) {
            if (i % k == 0) l[i] = nums[i];
            else l[i] = std::max(l[i - 1], nums[i]);
            
            int j = n - i - 1;
            if ((j + 1) % k == 0) r[j] = nums[j];
            else r[j] = std::max(r[j + 1], nums[j]);
        }
        
        res.resize(n - k  + 1);
        for (int i = 0; i < n - k + 1; i++)
            res[i] = std::max(l[i + k - 1], r[i]);
        return res;
    }

    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        std::vector<int> result;
        auto size = (int)nums.size();
        if (size && k > 0 && k <= size) {
            auto val = nums.at(0);
            auto max = val;
            auto first = val;
            for (auto i=1; i < k; ++i) {
                val = nums.at(i);
                if (val > max)
                    max = val;
            }
            result.push_back(max);

            auto limit = size - k + 1;
            for (auto i=1; i < limit; ++i) {
                val = nums.at(i);
                auto j = i + k;
                if (first == max) {
                    if (val == first - 1) {
                        first = val;
                        max = val;
                        val = nums.at(j-1);
                        if (val > max)
                            max = val;
                    } else {
                        first = val;
                        max = val;
                        for (auto w=i+1; w < j; ++w) {
                            val = nums.at(w);
                            if (val > max)
                                max = val;
                        }
                    }
                } else {
                    first = val;
                    if (val > max)
                        max = val;
                    val = nums.at(j-1);
                    if (val > max)
                        max = val;
                }
                result.push_back(max);
            }
        }
        return result;
    }
};

static void example(const std::vector<int> &arg, int k) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto nums(arg);
    print(nums, "nums");
    std::cout << "k=" << k << std::endl;
    Solution solution;
    auto result = solution.maxSlidingWindow(nums, k);
    print(result, "result");
}

static void f1(void) { example({1,3,-1,-3,5,3,6,7}, 3); }
static void f2(void) { example({1}, 1); }
static void f3(void) { example({1,-1}, 1); }
static void f4(void) { example({9,11}, 2); }
static void f5(void) { example({4,-2}, 2); }
static void f6(void) { example({7,2,4}, 2); }
static void f7(void) { example({1,3,1,2,0,5}, 3); }
static void f8(void) { example({9,10,9,-7,-4,-8,2,-6}, 5); }
static void f9(void) { example({1,3,-1,-3,5,3,6,7}, 3); }


static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8, f9
};

int main() {
    for (const auto& f : examples)
        f();
}
