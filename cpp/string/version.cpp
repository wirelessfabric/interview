// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm version.cpp -o version
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

class Solution {
public:
    string strip(string &str) {
        auto *s = str.c_str();
        string result;
        auto zerostrip = true;
        char c;
        while ((c = *s++)) {
            if (c == '.')
                zerostrip = true;
            else if (c == '0' && zerostrip)
                continue;
            else {
                result += c;
                zerostrip = false;
            }
        }
        return result;
    }

    void zeropad(string &str, int n) {
        for (auto i=0; i < n; ++i)
            str += '0';
    }

    int compareVersion(string version1, string version2) {
        if (version1 == version2)
            return 0;
        else if (version1 < version2)
            return -1;

        auto s1 = std::move(strip(version1));
        auto s2 = std::move(strip(version2));
        auto n1 = s1.size();
        auto n2 = s2.size();

        if (n1 < n2 && s2[n2-1] != '0')
            zeropad(s1, n2-n1);
        else if (n2 < n1 && s1[n1-1] != '0')
            zeropad(s2, n1-n2);

        if (s1 == s2)
            return 0;
        else if (s1 < s2)
            return -1;

        auto i1 = stoi(s1);
        auto i2 = stoi(s2);

        if (i1 == i2)
            return 0;
        else if (i1 < i2)
            return -1;
        else
            return 1;
    }
};

int main(int argc, char **argv) {
    if (argc == 3) {
        string version1 = std::string(argv[1]);
        string version2 = std::string(argv[2]);

        Solution solution;
        auto result = solution.compareVersion(version1, version2);
        std::cout << result << std::endl;
    }
}
