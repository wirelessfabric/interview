// SPDX-License-Identifier: MIT
//
// g++ -std=c++17 -O3 -I.. string_to_integer.cpp -o string_to_integer
// g++ --version 11.3.0 on soho x86_64 ubuntu 22.04

// code::dive conference 2015 - Andrei Alexandrescu - Writing Fast Code II
// https://www.youtube.com/watch?v=9tvbz8CSI8M

#include "common.h"
#include "print.h"
#include "speedup.h"

constexpr unsigned long error = std::numeric_limits<unsigned long>::max();

class Solution {
public:
    unsigned long atoui(const char* b, const char* e) {
        if (e < b)
            return error;
        static constexpr unsigned long pow10[] = {
            10000000000000000000UL,
            1000000000000000000UL,
            100000000000000000UL,
            10000000000000000UL,
            1000000000000000UL,
            100000000000000UL,
            10000000000000UL,
            1000000000000UL,
            100000000000UL,
            10000000000UL,
            1000000000UL,
            100000000UL,
            10000000UL,
            1000000UL,
            100000UL,
            10000UL,
            1000UL,
            100UL,
            10UL,
            1UL,
        };
        constexpr auto maxdigits = sizeof(pow10) / sizeof(*pow10);
        auto i = maxdigits - (e - b);
        if (i < 0)
            return error;
        unsigned long result = 0;
        for (; e - b >= 4;) {
            auto d0 = (unsigned long)(*b++) - '0';
            if (d0 >= 10)
                return error;
            auto d1 = (unsigned long)(*b++) - '0';
            if (d1 >= 10)
                return error;
            auto d2 = (unsigned long)(*b++) - '0';
            if (d2 >= 10)
                return error;
            auto d3 = (unsigned long)(*b++) - '0';
            if (d3 >= 10)
                return error;

            auto s0 = pow10[i++] * d0;
            auto s1 = pow10[i++] * d1;
            auto s2 = pow10[i++] * d2;
            auto s3 = pow10[i++] * d3;

            result += s0 + s1 + s2 + s3;
        }
        for (; b != e;) {
            auto digit = (unsigned long)(*b++) - '0';
            if (digit >= 10)
                return error;
            result += pow10[i++] * digit;
        }
        return result;
    }

    unsigned long atoui2(const char* b, const char* e) {
        if (e < b)
            return error;
        static constexpr unsigned long pow10[] = {
            10000000000000000000UL,
            1000000000000000000UL,
            100000000000000000UL,
            10000000000000000UL,
            1000000000000000UL,
            100000000000000UL,
            10000000000000UL,
            1000000000000UL,
            100000000000UL,
            10000000000UL,
            1000000000UL,
            100000000UL,
            10000000UL,
            1000000UL,
            100000UL,
            10000UL,
            1000UL,
            100UL,
            10UL,
            1UL,
        };
        constexpr auto maxdigits = sizeof(pow10) / sizeof(*pow10);
        auto i = maxdigits - (e - b);
        if (i < 0)
            return error;
        unsigned long result = 0;
        for (; b != e; ++b) {
            auto digit = (unsigned long)(*b) - '0';
            if (digit >= 10)
                return error;
            result += pow10[i++] * digit;
        }
        return result;
    }

    unsigned long atoui1(const char* b, const char* e) {
        if (e < b)
            return error;
        static constexpr unsigned long pow10[] = {
            10000000000000000000UL,
            1000000000000000000UL,
            100000000000000000UL,
            10000000000000000UL,
            1000000000000000UL,
            100000000000000UL,
            10000000000000UL,
            1000000000000UL,
            100000000000UL,
            10000000000UL,
            1000000000UL,
            100000000UL,
            10000000UL,
            1000000UL,
            100000UL,
            10000UL,
            1000UL,
            100UL,
            10UL,
            1UL,
        };
        constexpr auto maxdigits = sizeof(pow10) / sizeof(*pow10);
        auto i = maxdigits - (e - b);
        if (i < 0)
            return error;
        unsigned long result = 0;
        for (; b != e; ++b) {
            if (*b < '0' || *b > '9')
                return error;
            result += pow10[i++] * (*b - 0);
        }
        return result;
    }

    unsigned long atoui0(const char* b, const char* e) {
        if (b >= e)
            return error;
        unsigned long result = 0;
        for (; b != e; ++b) {
            if (*b < '0' || *b > '9')
                return error;
            result = result * 10 + (*b - '0');
        }
        return result;
    }
};

void atoui(const char* s, unsigned long expect) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    decltype(expect) result = 0;
    if (s && s[0]) {
        std::cout << "s=[" << s << "]" << std::endl;
        Solution solution;
        result = solution.atoui(s, &s[std::strlen(s)]);
    }
    if (result == error)
        std::cout << "result = error" << std::endl;
    else
        std::cout << "result = " << result << std::endl;
    if (result == expect)
        return;
    else if (expect == error)
        std::cout << "expect = error" << std::endl;
    else
        std::cout << "expect = " << expect << std::endl;
}

static void f1(void) { atoui("100", 100); }
static void f2(void) { atoui("9999", 9999); }
static void f3(void) { atoui("1234567890", 1234567890); }
static void f4(void) { atoui("0987654321", 987654321); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4
};

int main() {
    for (const auto& f : examples)
        f();
}
