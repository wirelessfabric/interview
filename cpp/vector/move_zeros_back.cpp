// SPDX-License-Identifier: MIT
// g++ -std=c++20 -O3 -lstdc++ -lm move_zeros_back.cpp -o move_zeros_back

#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>

using std::execution::par_unseq;

class Solution {
public:
    void move_zeros_back() {
        std::vector<int> A{0,1,5,3,2,67,0};
        std::vector<int> B;
        auto count{0};
        
        std::for_each(par_unseq, A.begin(), A.end(),
            [&](int index) {
                auto v = A[index];
                if (v)
                    B.emplace_back(v);
                else
                    count++;
            });

        A = B;
        for (auto i=0; i < count; ++i)
            A.emplace_back(0);
    }
};

int main(int argc, char **argv) {
    Solution S;
    S.move_zeros_back();
    return 0;
}
