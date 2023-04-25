// SPDX-License-Identifier: MIT
//
// g++ -std=c++2a -O3 popcount.cpp -o popcount
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// clang++ -std=c++2a -O3 popcount.cpp -o popcount
// Apple clang++ version 14.0.0 (clang-1400.0.29.202) on Apple M1 macOS Ventura 13.2.1
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 popcount.cpp
// cl version 19.35.32215 for x64

#include <iostream>
#include <stdint.h>
#include <concepts>
#include <bitset>
#include <bit>

// a.k.a Hamming Weight

int popcount(uint32_t i) {
     i -= (i >> 1) & 0x55555555;
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

//
// Lightning Talk: Finding Whether a Number is a Power of 2
// Ankur Satle @ CppCon 2022
// https://www.youtube.com/watch?v=Df-qEsWjzQw
//

constexpr bool pow2_std_has_single_bit(std::unsigned_integral auto u) { // @ 4:12
    return std::has_single_bit(u);
}

constexpr bool pow2_std_popcount(std::unsigned_integral auto u) { // @ 3:53
    return std::popcount(u) == 1;
}

constexpr auto popcnt_bitset(std::unsigned_integral auto u) { // @ 3:20
    std::bitset<sizeof(decltype(u))> bs(u);
    return bs.count();
}

constexpr bool pow2_bitset(std::unsigned_integral auto u) {
    return popcnt_bitset(u) == 1;
}

constexpr auto popcnt_concepts(std::unsigned_integral auto u) { // @ 2:08
    int popcnt{ 0 };
    for (auto i = 0u; i < sizeof(decltype(u)); ++i)
        if (u & (1u << i))
            ++popcnt;
    return popcnt;
}

constexpr bool pow2_concepts(std::unsigned_integral auto u) {
    return popcnt_concepts(u) == 1;
}

constexpr auto popcnt_constexpr(unsigned u) { // @ 1:21
    int popcnt{ 0 };
    for (auto i = 0u; i < sizeof(u); ++i)
        if (u & (1u << i))
            ++popcnt;
    return popcnt;
}

constexpr bool pow2_constexpr(unsigned u) {
    return popcnt_constexpr(u) == 1;
}

// Matt Godbolt: https://www.youtube.com/watch?v=w0sz5WbS5AM @ 39:49
// -O2 -march=haswell -Wall -Wextra -pedantic
// int popcnt(unsigned int)
//   xor eax,eax
//   popcnt eax, edit
//   ret
int popcnt(uint32_t n) {
    int count = 0;
    while (n != 0) {
        count++;
        n &= (n - 1);
    }
    return count;
}

// -O2 -march=haswell -Wall -Wextra -pedantic
// bool pow2_popcnt(uint32_t n)
//  test    edi, edi
//  je      .L5
//  popcnt  edi, edi
//  cmp     edi, 1
//  sete    al
//  ret
// .L5:
//  xor     eax, eax
//  ret
bool pow2_popcnt(uint32_t n) {
    return popcnt(n) == 1;
}

// Matt Godbolt: https://www.youtube.com/watch?v=w0sz5WbS5AM @ 41:15
// -O2 -march=haswell -Wall -Wextra -pedantic
// bool pow2_blsr(uint32_t n)
//  xor     eax, eax
//  test    edi, edi
//  je      .L6
//  blsr    eax, edi
//  sete    al
// .L6:
//  ret
bool pow2_blsr(uint32_t n) {
    return n && (n & (n - 1)) == 0;
}

int main(int argc, char *argv[]) {
    unsigned u{ 0 };

    if (argc > 1) {
        sscanf(argv[1], "%u", &u);
    } else {
        printf("Input a decimal value or 0 to end: ");
        if (scanf("%u", &u) != 1) {
            printf("scanf() ERROR");
            return -1;
        }
    }

    std::cout << "Number of set bits in " << u << " (0x" << std::hex << u << ")" << std::endl;

    std::cout << "std::popcount() is " << std::popcount(u) << std::endl;

    std::cout << "popcount() is " << popcount(u) << std::endl;

    std::cout << "popcnt() is " << popcnt(u) << std::endl;

    std::cout << "popcnt_constexpr() is " << popcnt_constexpr(u) << std::endl;

    std::cout << "popcnt_concepts() is " << popcnt_concepts(u) << std::endl;

    std::cout << "popcnt_bitset() is " << popcnt_bitset(u) << std::endl;

    std::cout << "pow2_blsr() is " << pow2_blsr(u) << std::endl;

    std::cout << "pow2_std_popcount() is " << pow2_std_popcount(u) << std::endl;

    std::cout << "pow2_std_has_single_bit() is " << pow2_std_has_single_bit(u) << std::endl;

    std::cout << "pow2_constexpr() is " << pow2_constexpr(u) << std::endl;

    std::cout << "pow2_concepts() is " << pow2_concepts(u) << std::endl;

    std::cout << "pow2_bitset() is " << pow2_bitset(u) << std::endl;

    return 0;
}
