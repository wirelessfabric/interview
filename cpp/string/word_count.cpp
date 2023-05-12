// SPDX-License-Identifier: MIT
//
// g++ -std=c++17 -O3 word_count.cpp -o word_count -ltbb
// g++ --version 11.3.0 on soho x86_64 ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. word_count.cpp
// cl version 19.35.32215 on gogo x64 windows 11

// C++ Standard Parallelism - Bryce Adelstein Lelbach - CppCon 2021
// https://www.youtube.com/watch?v=KK3JXvSiJG4
//
// C++ Standard Parallelism - Bryce Adelstein Lelbach - GTC 2023
// https://www.youtube.com/watch?v=nwrgLH5yAlM

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <execution>
#include <numeric>

// namespace this_thread = std::this_thread;

namespace ex = std::execution;

class Solution {
public:
    std::size_t word_count(std::string_view s) {
        if (s.empty())
            return 0;

        return std::transform_reduce(ex::par_unseq,
            begin(s), end(s) - 1, begin(s) + 1,
            std::size_t(!std::isspace(s.front()) ? 1 : 0),
            std::plus(),
            [] (char l, char r) { return std::isspace(l) && !std::isspace(r); }
        );
    }
};

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string str;
        for (auto i=1; i < argc; i++) {
            if (i > 1)
                str += " ";
            str += std::string(argv[i]);
        }

        Solution S;
        std::cout << "# of words: " << S.word_count(str) << std::endl;
    }
}
