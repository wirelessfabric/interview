// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -lstdc++ -lm word_count.cpp -o word_count
// g++ --version 9.3.0

// C++ Standard Parallelism - Bryce Adelstein Lelbach - CppCon 2021
// https://www.youtube.com/watch?v=KK3JXvSiJG4

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <execution>
#include <numeric>

// namespace thid_thread = std::this_thread;

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
