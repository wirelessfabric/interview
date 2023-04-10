// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm partition.cpp -o partition
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

class Solution {
public:
    vector<int> partitionLabels(string S) {
        vector<int> v;

        cout << "S: " << S << endl;
        auto s = S;
        auto size = s.size();
        for (size_t i=0; i < size; ++i) {
            string sub = s.substr(i,size);
            cout << "s{" << s << "} sub{" << sub << "}" << endl;

            auto c = s[i];
            auto pos = s.find_last_of(c);
            cout << "pos=" << pos << " i=" << i << endl;
            if (pos == i) {
                pos = s.find_last_not_of(c);
                v.push_back(i+1);
                s = s.substr(i+1, size);
                size = s.size();
                i = -1;
                continue;
            }

            i = pos;
        }
        return v;
    }
};

int main(int argc, char **argv) {
    if (argc > 1) {
        Solution S;
        vector<int> v = S.partitionLabels(string(argv[1]));
        cout << "result" << endl;
        for (auto &i : v)
            cout << i << endl;
    }
    return 0;
}
