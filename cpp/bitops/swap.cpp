// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -o swap swap.cpp

#include <cstdint>

// Matt Godbolt: https://www.youtube.com/watch?v=w0sz5WbS5AM&t=1356s
// x86_64-pc-linux-gnu-gcc-10.2.0 -std=c++2a -O3 -o swap swap.cpp 
// switchBits(unsigned int)
//   mov rd8, eax
//   bswap rd8
//   ret
static inline uint32_t swap32_matt_godbolt(uint32_t x) {
    auto first  = (x >>  0) & 0xff;
    auto second = (x >>  8) & 0xff;
    auto third  = (x >> 16) & 0xff;
    auto fourth = (x >> 24) & 0xff;
    return (first  << 24) |
           (second << 16) |
           (third  <<  8) |
           (fourth <<  0);
}

//   mov rd8, eax
//   bswap rd8
//   ret
static inline uint32_t swap32(uint32_t x) {
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) <<  8) |
           ((x & 0x00FF0000) >>  8) |
           ((x & 0xFF000000) >> 24);
}

int main() {
    return swap32(0x01234567);
}