// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -I.. -lstdc++ -lm intersect.cpp -o intersect
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    std::vector<int> intersection(std::vector<int>& nums1, std::vector<int>& nums2) {
        std::unordered_map<int, int> hash1, hash2;
        std::vector<int> result;

        for (const auto &n1 : nums1)
            hash1[n1]++;

        std::cout << "hash1" << std::endl;
        print(hash1);

        for (const auto &n2 : nums2) {
            if (hash1.find(n2) != hash1.end())
                hash2[n2]++;
        }

        std::cout << "hash2" << std::endl;
        print(hash2);

        for (const auto &h2 : hash2)
            result.push_back(h2.first);

        return result;
    }
};

int main() {
    Solution solution;
    std::vector<int> nums1{4,9,5};
    std::vector<int> nums2{9,4,9,8,4};

    std::cout << "nums1 ";
    print(nums1);
    std::cout << " nums2 ";
    print(nums2);
    std::cout << std::endl;

    auto output = solution.intersection(nums1, nums2);

    std::cout << "result " << std::endl;
    print(output);
    std::cout << std::endl;
}
