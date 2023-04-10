// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm repString.cpp -o repString
// g++ --version 9.3.0

#include <iostream>
#include <string>

class Solution {
public:
    bool repeatedSubstringPattern(std::string s) {
        auto size = s.size();
        if (size == 1)
            return false;

        for (auto i=1; i < size; i++) {
            std::string sub = s.substr(0, i);
            auto subsize = sub.size();

            if (sub == s)
                return false;

            if (size % subsize)
                continue;

            auto count = size / subsize;

            if (subsize * 2 > size)
                return false;

            std::string ss = sub;
            for (auto n=0; n < count; n++) {
                ss += sub;
                if (ss == s)
                    return true;
            }
        }

        return false;
    }
};

int main(int argc, char **argv) {
    if (argc == 2) {
        Solution S;
        if (S.repeatedSubstringPattern(std::string(argv[1])))
            std::cout << "True" << std::endl;
        else
            std::cout << "False" << std::endl;
    }
}
