// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm removeDuplicateLetters.cpp -o removeDuplicateLetters
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <vector>
#include <map>

static const auto speedup = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

static inline int bitmask(char c) {
    return 1 << (c - 'a');
}

static inline int bitmap(const std::string &s) {
    auto map = 0;
    for (auto &c : s) {
        if (c < 'a' || c > 'z')
            continue;
        auto bit = bitmask(c);
        if (bit & map)
            continue;
        map |= bit;
    }
    return map;
}

static inline char smallest(int map, char c) {
    while (map && c <= 'z') {
        if (map & bitmask(c))
            return c;
        c++;
    }
    return '\0';
}

class Solution {
public:
    std::string removeDuplicateLetters(std::string s) {
        std::string result;
        auto map = bitmap(s);
        auto c = 'a';
        while (map) {
            if (map & 1)
                result += c;
            map >>= 1;
            c++;
        }
        return result;
    }
};

void f(const std::string &arg) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::string s(arg);
    std::cout << "s = {" << s << "}" << std::endl;
    auto result = solution.removeDuplicateLetters(s);
    std::cout << "result = {" << result << "}" << std::endl;
}

static void f1(void) { f("bcabc"); }
static void f2(void) { f("cbacdcbc"); }

static std::vector<void (*)(void)> examples {
    f1, f2
};

int main() {
    for (const auto& f : examples)
        f();
}
