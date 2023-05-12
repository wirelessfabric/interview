// SPDX-License-Identifier: MIT
//
// g++ -std=c++17 -O3 -I.. get_skyline.cpp -o get_skyline
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    std::vector<std::vector<int>> getSkyline(std::vector<std::vector<int>>& buildings) {
        std::map<int,int> m;
        for (const auto& v : buildings) {
            auto l = v[0];
            auto r = v[1];
            auto h = v[2];
            if (r == INT_MAX)
                m[l] = h;
            else for (auto i=l; i <= r; ++i) {
                auto it = m.find(i);
                if (it == m.end() || h > it->second)
                    m[i] = h;
            }
        }

        std::vector<std::vector<int>> result;
        auto xs = 0;
        auto h = 0;
        auto n = 0;
        for (const auto& it : m) {
            auto x = it.first;
            auto y = it.second;
            if (result.size()) {
                if (x > (xs + 1)) {
                    result.push_back({xs,0});
                    h = 0;
                    n = 0;
                }
                if (y > h)
                    result.push_back({x,y});
                else if (y < h)
                    result.push_back({x-1,y});
            } else {
                result.push_back({x,y});
            }
            h = y;
            xs = x;
            n++;
        }
        if (result.size()) {
            if (xs == 0)
                xs = INT_MAX;
            else if (n == 1)
                xs++;
            result.push_back({xs,0});
        }
        return result;
    }
};

static void example(const std::vector<std::vector<int>> &arg) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto buildings(arg);
    print(buildings, "buildings");
    Solution solution;
    auto result = solution.getSkyline(buildings);
    print(result, "result");
}

static void f1(void) { example({{2,9,10},{3,7,15},{5,12,12},{15,20,10},{19,24,8}}); }
static void f2(void) { example({{0,2147483647,2147483647}}); }
static void f3(void) { example({{1,2,1},{2147483646,2147483647,2147483647}}); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3
};

int main() {
    for (const auto& f : examples)
        f();
}
