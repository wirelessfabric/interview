// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm buddyStrings.cpp -o buddyStrings
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

static inline int popcount(int n) {
    int count = 0;
    while (n != 0) {
        count++;
        n &= (n - 1);
    }
    return count;
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
    bool swapTwoLetters(std::string &A, std::string &B, int mapA) {
        if (mapA != bitmap(B))
            return false;

        const auto size = (int)A.size();
        for (auto i=0; i < size; ++i) {
            auto a = A[i];
            for (auto j=i+1; j < size; ++j) {
                A[i] = A[j];
                A[j] = a;
                if (A == B)
                    return true;
                A[j] = A[i];
            }
            A[i] = a;
        }

        return false;
    }

    bool buddyStrings(std::string A, std::string B) {
        const auto size = (int)A.size();
        if (size != (int)B.size() || size < 2)
            return false;

        if (A == B) {
            auto mapA = bitmap(A);
            return popcount(mapA) == 1 ? true : size == 2 ? false : swapTwoLetters(A, B, mapA);
        }

        std::string X, Y;
        for (auto i=0; i < size; ++i) {
            if (A[i] != B[i]) {
                X += A[i];
                Y += B[i];
            }
        }

        return swapTwoLetters(X, Y, bitmap(X));
    }
};

void f(const std::string &arg1, const std::string &arg2) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::string s1(arg1), s2(arg2);
    std::cout << "s1={" << s1 << "}  s2={" << s2 << "}" << std::endl;
    auto result = solution.buddyStrings(s1, s2);
    std::cout << "result = ";
    if (result)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
}

static void f1(void) { f("ab", "ba"); }
static void f2(void) { f("ab", "ab"); }
static void f3(void) { f("aa", "aa"); }
static void f4(void) { f("aaaaaaabc", "aaaaaaacb"); }
static void f5(void) { f("", "aa"); }
static void f6(void) { f("abab", "abab"); }
static void f7(void) { f("abcd", "abcd"); }
static void f8(void) { f("abcd", "badc"); }
static void f9(void) { f("abcd", "bacd"); }
static void f10(void) { f("abcd", "abdc"); }
static void f11(void) { f("abab", "abab"); }
static void f12(void) { f("a", "a"); }
static void f13(void) { f("ab", "babbb"); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13
};

int main() {
    for (const auto& f : examples)
        f();
}
