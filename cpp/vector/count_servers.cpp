// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. count_servers.cpp -o count_servers
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++17 /I.. count_servers.cpp
// cl version 19.35.32215 for x64

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int countServers(std::vector<std::vector<int>>& grid) {
        const size_t m{ grid.size() };
        const size_t n{ grid[0].size() };
        std::vector<int> sum_columns(n, 0);
        auto count{ 0 };

        for (const auto& row : grid) {
            auto sum{ 0 };
            for (auto col = 0; col < n; ++col) {
                if (row[col]) {
                    sum++;
                    sum_columns[col]++;
                }
            }
            if (sum > 1)
                count += sum;
        }

        for (const auto& s : sum_columns)
            if (s > 1)
                count += s - 1;

        return count;
    }

    int countServersBitmap(std::vector<std::vector<int>>& grid) {
        const size_t m{ grid.size() };
        const size_t n{ grid[0].size() };
        auto count{ 0 };

        uint64_t bits[128];
        auto bitmap{ (uint8_t*)bits };
        for (auto row=0; row < m; ++row) {
            const auto& data{ grid[row] };
            auto sum{ 0 };
            auto bitmask{ 0x80 };
            *bitmap = 0;
            for (auto col=0; col < n; ++col) {
                if (data[col]) {
                    sum++;
                    *bitmap |= bitmask;
                }
                if (bitmask == 1) {
                    bitmask = 0x80;
                    bitmap++;
                    *bitmap = 0;
                }
                else
                    bitmask >>= 1;
            }
            bitmap++;
            if (sum > 1)
                count += sum;
        }

        bitmap = (uint8_t*)bits;
        auto row{ m };
        std::cout << ", {";
        while (row) {
            auto col{ (n / 8) + 1 };
            std::cout << "{ ";
            while (col) {
                auto byte{ (uint32_t)*bitmap };
                std::cout << "0x" << std::hex << byte;
                if (--col)
                    std::cout << ", ";
                bitmap++;
            }
            std::cout << " }";
            if (--row)
                std::cout << ", ";
        }
        std::cout << "}";

        return count;
    }

    int countServersIncomplete(std::vector<std::vector<int>>& grid) {
        const size_t m{ grid.size() };
        const size_t n{ grid[0].size() };
        auto count{ 0 };

        for (const auto& row : grid) {
            auto sum { std::reduce(std::execution::par_unseq, row.begin(), row.end(), 0, std::plus<>()) };
            if (sum > 1)
                count += sum;
        }

        return count;
    }
};

void example(const std::vector<std::vector<int>> &candidates, int target) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << ": ";
    Solution solution;
    print(candidates);
    std::cout << ", target = " << target;

    auto grid(candidates);
    auto result = solution.countServers(grid);
    std::cout << ", result = " << result;
    if (result == target)
        std::cout << " pass" << std::endl;
    else
        std::cout << " fail" << std::endl;
}

void f1(void) { example({{1,0},{0,1}}, 0); }
void f2(void) { example({{1,0},{1,1}}, 3); }
void f3(void) { example({{1,1,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,1}}, 4); }
void f4(void) { example({{1,0,0},{1,1,1}}, 4); }
void f5(void) { example({{1,0,0,0},{1,1,0,1}}, 4); }
void f6(void) { example({{0,0,1},{1,1,1}}, 4); }
void f7(void) { example({{0,0,0,1},{1,1,0,1}}, 4); }
void f8(void) { example({{0,0,0,1,0,0,0,0,0},{1,1,0,1,0,0,0,0,0}}, 4); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8
};

int main() {
    for (const auto& f : examples)
        f();
}
