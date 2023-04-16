// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm lastWordLen.cpp -o lastWordLen
// g++ --version 9.3.0

#include <iostream>
#include <string>

class Solution {
public:
    int lengthOfLastWord(std::string s) {
        auto size = s.size();
        if (!size)
            return 0;
        auto alpha = s.find_last_not_of(" ");
        if (alpha == std::string::npos)
            return 0;
        auto space = s.find_last_of(" ", alpha);
        if (space == std::string::npos)
            return alpha + 1;
        return alpha - space;
    }
};

static void example(std::string &s) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "s = " << s << std::endl;
    auto result = solution.lengthOfLastWord(s);
    std::cout << "result " << result << std::endl;
}

void example1(void) {
    std::string s{"Hello World"};
    example(s);
}

void example2(void) {
    std::string s{"Hello World "};
    example(s);
}

void example3(void) {
    std::string s{" "};
    example(s);
}

void example4(void) {
    std::string s{"          "};
    example(s);
}

void example5(void) {
    std::string s{"Hello"};
    example(s);
}

void example6(void) {
    std::string s{"a "};
    example(s);
}

void example7(void) {
    std::string s{"day    "};
    example(s);
}

int main(int argc, char **argv) {
    example1();
    example2();
    example3();
    example4();
    example5();
    example6();
    example7();
}
