// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm seqDigits.cpp -o seqDigits
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <climits>

class Solution {
public:
    std::vector<int> sequentialDigits(int low, int high) {
        const auto max = 123456789;
        if (high > max)
            high = max;

        const std::string seq = std::to_string(max);
        const auto seqlen = seq.size();
        const auto lowlen = std::to_string(low).size();
        const auto highlen = std::to_string(high).size();
        const auto width = std::max(lowlen, highlen);

        std::vector<int> result;
        for (auto len=lowlen; len <= highlen; ++len) {
            for (auto off=0; off < seqlen; ++off) {
                std::string sub = seq.substr(off, len);
                if (sub.size() < len)
                    break;

                auto val = std::stoi(sub);
                if (val < low)
                    continue;
                if (val > high)
                    break;

                result.push_back(val);
            }
        }
        return result;
    }

    void print(const std::vector<int> &v, int start, int end) {
        std::cout << "[";
        int n = 0;
        for (auto i=start; i <= end; ++i) {
            if (n++)
                std::cout << ",";
            std::cout << v[i];
        }
        std::cout << "]";
    }

    void print(const std::vector<int> &v) {
        std::cout << "[";
        int n = 0;
        for (const auto &e : v) {
            if (n++)
                std::cout << ",";
            std::cout << e;
        }
        std::cout << "]";
    }

    void print(const std::vector<std::vector<int>> &vv) {
        std::cout << "[";
        int n = 0;
        for (const auto &v : vv) {
            if (n++)
                std::cout << ",";
            print(v);
        }
        std::cout << "]";
    }
};

void example(int low, int high) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    std::cout << "low = " << low << " high = " << high << std::endl;
    Solution solution;
    auto result = solution.sequentialDigits(low, high);
    std::cout << "result = ";
    solution.print(result);
    std::cout << std::endl;
}

void example1(void) { example(100, 300); }
void example2(void) { example(1000, 13000); }
void example3(void) { example(10, 1000000000); }
void example4(void) { example(58, 155); }

int main(int argc, char **argv) {
    example1();
    example2();
    example3();
    example4();
}
