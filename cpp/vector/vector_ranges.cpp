// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm xorMax.cpp -o xorMax
// g++ --version 9.3.0

// C++ Standard Parallelism - Bryce Adelstein Lelbach - CppCon 2021
// https://www.youtube.com/watch?v=KK3JXvSiJG4

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <execution>
#include <ranges>

using std::execution::par_unseq;

class Solution {
public:
    void iterate_ranges(std::vector<int> x) {
        auto f = [] (auto& obj) { std::cout << obj << ", "; };
        std::for_each(par_unseq, begin(x), end(x), f);
        std::cout << std::endl;
    }

    void iterate_iota(std::vector<int> x) {
        auto i = std::views::iota(0, (int)x.size());
        auto f = [&] (auto index) { std::cout << x[index] << ", "; };
        std::for_each(par_unseq, i.begin(), i.end(), f);
        std::cout << std::endl;
    }

    void iterate_filtered(std::vector<int> x) {
        auto f = [] (auto& obj) { return obj & 1; };
        auto v = std::views::filter(x, f);
        auto n = std::reduce(par_unseq, begin(v), end(v));
        std::cout << n << std::endl;
    }

    void iterate_disjoint(std::vector<int> x) {
        auto i = std::views::iota(0, (int)x.size());
        auto f1 = [ ] (auto index) { std::cout << "x[" << index << "]"; };
        auto f2 = [&] (auto index) { std::cout << "=" << x[index]; };
        std::for_each(par_unseq, i.begin(), i.end(), f1);
        std::for_each(par_unseq, i.begin(), i.end(), f2);
        std::cout << std::endl;
    }

    void sort_disjoint(std::vector<int> x) {
        auto f = [] (auto& obj) { std::cout << obj << ","; };
        std::sort(par_unseq, begin(x), end(x));
        auto last = std::unique(par_unseq, begin(x), end(x));
        x.erase(last, end(x));
        std::for_each(par_unseq, begin(x), end(x), f);
        std::cout << std::endl;
    }

    void iterate_fused(std::vector<int> x) {
        auto i = std::views::iota(0, (int)x.size());
        auto f1 = [ ] (auto index) { std::cout << "x[" << index << "]"; };
        auto f2 = [&] (auto index) { std::cout << "=" << x[index]; };
        auto v = std::views::transform(x, f1);
        std::for_each(par_unseq, i.begin(), i.end(), f2);
        std::cout << std::endl;
    }
};

int main(int argc, char **argv) {
    const std::vector v{1,2,3,1,5,3,2,4,5,6};
    Solution S;
    S.iterate_ranges(v);
    S.iterate_iota(v);
    S.iterate_filtered(v);
    S.iterate_disjoint(v);
    S.iterate_fused(v);
    S.sort_disjoint(v);
    return 0;
}
