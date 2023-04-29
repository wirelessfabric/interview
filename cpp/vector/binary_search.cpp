// SPDX-License-Identifier: MIT
// g++ -std=c++20 -O3 -I.. -lstdc++ -lm binary_search.cpp -o binary_search
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int binary_search(std::vector<int> &nums, int target) {
        auto base = 0;
        auto size = (int)nums.size();
        while (size) {
            auto half = size / 2;
            auto mid = base + half;
            auto val = nums[mid];
            if (val == target)
                return mid;

            if (val < target) {
                base = mid + 1;
                if ((size & 1) == 0)
                    half--;
            }
            size = half;
        }
        return -1;
    }

    int binary_search_cpp20_std_midpoint(std::vector<int>& nums, int target) {
        return std::lower_bound(nums.begin(), nums.end(), target) - nums.begin();
    }

    int binary_search_cpp20_std_lower_bound(std::vector<int>& nums, int target) {
        const auto first = nums.begin();
        const auto last = nums.end();
        const auto it = std::lower_bound(first, last, target);
        return it == last ? it - first : -1;
    }
};

static void example(const std::vector<int> &arg, int target) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto nums(arg);
    std::cout << "nums = ";
    print(nums);
    std::cout << ", find " << target << std::endl;
    Solution solution;
    auto index = solution.binary_search(nums, target);
    std::cout << "index = " << index << std::endl;
}

static void f1(void) { example({-1,0,3,5,9,12}, 9); }
static void f2(void) { example({-1,0,3,5,9,12}, 2); }
static void f3(void) { example({-1,0,3,5,9,12}, -1); }
static void f4(void) { example({-1,0,3,5,9,12}, 0); }
static void f5(void) { example({-1,0,3,5,9,12}, 3); }
static void f6(void) { example({-1,0,3,5,9,12}, 5); }
static void f7(void) { example({-1,0,3,5,9,12}, 12); }
static void f8(void) { example({5}, -5); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8
};

int main() {
    for (const auto& f : examples)
        f();
}


// Plenary: Six Impossible Things - Kevlin Henney - CppCon 2021
// https://www.youtube.com/watch?v=YoaZzIZFErI
// @ 14:56
//
// int binary_search_with_overflow(int x, const int *a, int n) {
//     int middle, left=0, right=n-1;
//     if (x <= a[left]) return 0;
//     if (x > a[righ]) return n;
//     while (right - left > 1) {
//         middle = (right + left) / 2;    // overflows at MAX_INT
//         (x <= a[middle] ? right : left) = middle;
//     }
//     return right;
// }

// Plenary: Six Impossible Things - Kevlin Henney - CppCon 2021
// https://www.youtube.com/watch?v=YoaZzIZFErI
// @ 14:56
//
// int binary_search_no_overflow(int x, const int *a, int n) {
//     int middle, left=0, right=n-1;
//     if (x <= a[left]) return 0;
//     if (x > a[righ]) return n;
//     while (right - left > 1) {
//         middle = left + (right + left) / 2;
//         (x <= a[middle] ? right : left) = middle;
//     }
//     return right;
// }

// Plenary: Six Impossible Things - Kevlin Henney - CppCon 2021
// https://www.youtube.com/watch?v=YoaZzIZFErI
// @ 15:04
//
// int binary_search_cpp20_std_midpoint(int x, const int *a, int n) {
//     int middle, left=0, right=n-1;
//     if (x <= a[left]) return 0;
//     if (x > a[righ]) return n;
//     while (right - left > 1) {
//         middle = std::midpoint(left, right);
//         (x <= a[middle] ? right : left) = middle;
//     }
//     return right;
// }

// Plenary: Six Impossible Things - Kevlin Henney - CppCon 2021
// https://www.youtube.com/watch?v=YoaZzIZFErI
// @ 15:14
//
// int binary_search_cpp20_std_lower_bound(int x, const int *a, int n) {
//    return std::lower_bound(a, a + n, x) - a;
// }

// Plenary: Six Impossible Things - Kevlin Henney - CppCon 2021
// https://www.youtube.com/watch?v=YoaZzIZFErI
// @ 15:19
//
// std::lower_bound(a, a + n, x);

// Optimizing Binary Search - Sergey Slotin - CppCon 2022
// https://www.youtube.com/watch?v=1RIPMQQRBWk
// @ 4:55
//
// int lower_bound_cppcon2022_sergey_slotin1(int x) {
//     int l = 0, r = n - 1;
//     while (l < r) {
//         int m = (l + r) / 2;
//         if (t[m] >= x)
//             r = m;
//         else
//             l = m + 1;
//     }
//     return l;
// }

// Optimizing Binary Search - Sergey Slotin - CppCon 2022
// https://www.youtube.com/watch?v=1RIPMQQRBWk
// @ 13:33
//
// int lower_bound_cppcon2022_sergey_slotin2(int x) {
//     int *base = t, len = n;
//     while (len > 1) {
//         int half = len / 2;
//         if (base[half - 1] < x) {
//             base += half;
//             len = len - half; // = ceil(len / 2)
//         } else {
//             len = half; // = floor(len / 2) 
//         }
//     }
//     return *base;
// }

// Optimizing Binary Search - Sergey Slotin - CppCon 2022
// https://www.youtube.com/watch?v=1RIPMQQRBWk
// @ 14:52
//
// int lower_bound_cppcon2022_sergey_slotin3(int x) {
//     int *base = t, len = n;
//     while (len > 1) {
//         int half = len / 2;
//         if (base[half - 1] < x)
//             base += half;
//         len -= half; // = ceil(len / 2)
//     }
//     return *base;
// }

// Optimizing Binary Search - Sergey Slotin - CppCon 2022
// https://www.youtube.com/watch?v=1RIPMQQRBWk
// @ 15:29
//
// int lower_bound_cppcon2022_sergey_slotin_branchless(int x) {
//     int *base = t, len = n;
//     while (len > 1) {
//         int half = len / 2;
//         base += (base[half - 1] < x) * half; // will be replaced with a "cmov"
//         len -= half;
//     }
//     return *base;
// }

// Optimizing Binary Search - Sergey Slotin - CppCon 2022
// https://www.youtube.com/watch?v=1RIPMQQRBWk
// @ 16:54
//
// int lower_bound_cppcon2022_sergey_slotin_branchless_prefech(int x) {
//     int *base = t, len = n;
//     while (len > 1) {
//         int half = len / 2;
//         len -= half;
//         __builtin_prefetch(&base[len / 2 - 1]);         // middle of the left half
//         __builtin_prefetch(&base[half + len / 2 - 1]);  // middle of the right half
//         base += (base[half - 1] < x) * half;
//     }
//     return *base;
// }

// https://github.com/sslotin/amh-code/tree/main/binsearch
// https://en.algorithmica.org/hpc/data-structures/binary-search/
