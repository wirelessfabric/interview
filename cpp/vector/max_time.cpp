// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm maxTime.cpp -o maxTime
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iomanip>

// https://leetcode.com/problems/largest-time-for-given-digits/solution/

class Solution {
public:
    std::string largestTimeFromDigits(std::vector<int>& A) {
        auto maxtime = -1;

        // prepare for the generation of permutations next
        std::sort(A.begin(), A.end());

        do {
            auto hour = A[0] * 10 + A[1];
            auto minute = A[2] * 10 + A[3];

            if (hour < 24 && minute < 60) {
                auto newtime = hour * 60 + minute;
                maxtime = newtime > maxtime ? newtime : maxtime;
            }
        } while(std::next_permutation(A.begin(), A.end()));

        if (maxtime == -1)
            return "";

        std::ostringstream strstream;
        strstream << std::setw(2) << std::setfill('0') << maxtime / 60
            << ":" << std::setw(2) << std::setfill('0') << maxtime % 60;
        return strstream.str();
    }
};

int main(int argc, char **argv) {
    Solution S;
    std::vector<int> A;
    for (auto i=1; i < argc; ++i)
        A.push_back(std::stoi(std::string(argv[i])));
    std::string s = S.largestTimeFromDigits(A);
    std::cout << s << std::endl;
}
