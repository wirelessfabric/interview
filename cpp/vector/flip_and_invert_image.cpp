// SPDX-License-Identifier: MIT
//
// g++ -std=c++17 -O3 -I.. flip_and_invert_image.cpp -o flip_and_invert_image
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    std::vector<std::vector<int>> flipAndInvertImage(std::vector<std::vector<int>>& A) {
        const int n = (int)A.at(0).size();
        std::vector<std::vector<int>> B(A);
        for (auto &v : B) {
            for (auto i=0, j=n-1; i < j; ++i, --j)
                std::swap(v[i], v[j]);
            for (auto& i : v)
                i ^= 1;
        }
        return std::move(B);
    }
};

void flipAndInvertImage(const std::vector<std::vector<int>> &arg) {
    static auto counter = 1;
    std::cout << "flipAndInvertImage" << counter++ << std::endl;
    Solution solution;
    auto A(arg);
    print(A, "A");
    auto result = solution.flipAndInvertImage(A);
    print(result, "result");
}

static void f1(void) { flipAndInvertImage({{1,1,0},{1,0,1},{0,0,0}}); }
static void f2(void) { flipAndInvertImage({{1,1,0,0},{1,0,0,1},{0,1,1,1},{1,0,1,0}});}

static std::vector<void (*)(void)> examples {
    f1, f2
};

int main() {
    for (const auto& f : examples)
        f();
}
