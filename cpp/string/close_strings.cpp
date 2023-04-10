// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -lstdc++ -lm closeStrings.cpp -o closeStrings
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

static const auto speedup = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

void print_begin(const char *text = nullptr) {
    if (text)
        std::cout << text << " = ";
    std::cout << "[";
}

void print_end(const char *text = nullptr) {
    std::cout << "]";
    if (text)
        std::cout << std::endl;
}

template<typename T, size_t N>
void print(const std::array<T, N> &a, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &e : a) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

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

static constexpr auto alphabetSize = 26;

static int freqmap(const std::string &s, std::array<int, alphabetSize> &freq) {
    for (auto &f : freq)
        f = 0;
    auto map = 0;
    for (const auto &c : s) {
        if (c < 'a' || c > 'z')
            continue;
        const auto letter = c - 'a';
        freq[letter]++;
        const auto bit = 1 << letter;
        if (bit & map)
            continue;
        map |= bit;
    }
    return map;
}

static inline int popcount(int i) {
    int count = 0;
    while (i != 0) {
        count++;
        i &= (i - 1);
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
    bool closeStrings(std::string A, std::string B) {
        const auto size = (int)A.size();
        if (size != (int)B.size())
            return false;

        std::array<int, alphabetSize> freqA;
        auto mapA = freqmap(A, freqA);

        std::array<int, alphabetSize> freqB;
        auto mapB = freqmap(B, freqB);

        if (mapA != mapB)
            return false;

        std::sort(freqA.begin(), freqA.end());
        std::sort(freqB.begin(), freqB.end());
        return freqA == freqB;
    }
};

void f(const std::string &arg1, const std::string &arg2) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::string s1(arg1), s2(arg2);
    std::cout << "s1={" << s1 << "}  s2={" << s2 << "}" << std::endl;
    auto result = solution.closeStrings(s1, s2);
    std::cout << "result = ";
    if (result)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
}

void f1(void) { f("ab", "ba"); }
void f2(void) { f("ab", "ab"); }
void f3(void) { f("aa", "aa"); }
void f4(void) { f("aaaaaaabc", "aaaaaaacb"); }
void f5(void) { f("", "aa"); }
void f6(void) { f("abab", "abab"); }
void f7(void) { f("abcd", "abcd"); }
void f8(void) { f("abcd", "badc"); }
void f9(void) { f("abcd", "bacd"); }
void f10(void) { f("abcd", "abdc"); }
void f11(void) { f("abab", "abab"); }
void f12(void) { f("a", "a"); }
void f13(void) { f("ab", "babbb"); }
void f14(void) { f("uau", "ssx"); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14
};

int main() {
    for (auto &f : examples)
        f();
}
