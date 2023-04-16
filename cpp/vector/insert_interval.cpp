// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. -lstdc++ -lm insert_interval.cpp -o insert_interval
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    std::vector<std::vector<int>> insert(std::vector<std::vector<int>> &intervals, std::vector<int> &newInterval) {
        std::vector<std::vector<int>> result;

        for (auto &interval : intervals) {
            if (interval[1] < newInterval[0]) {
                result.push_back(interval);
            } else if (interval[0] > newInterval[1]) {
                result.push_back(newInterval);
                newInterval = interval;
            } else if (interval[1] >= newInterval[0] || interval[0] <= newInterval[1]) {
                newInterval[0] = std::min(interval[0], newInterval[0]);
                newInterval[1] = std::max(interval[1], newInterval[1]);
            }
        }

        result.push_back(newInterval);
        return result;
    }
};

static void example(std::vector<std::vector<int>> &intervals, std::vector<int> &newInterval) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "intervals = ";
    print(intervals);
    std::cout << " newInterval = ";
    print(newInterval);
    std::cout << std::endl;
    auto result = solution.insert(intervals, newInterval);
    std::cout << "result ";
    print(result);
    std::cout << std::endl;
}

void example1(void) {
    std::vector<std::vector<int>> intervals{{1,3},{6,9}};
    std::vector<int> newInterval{2,5};
    example(intervals, newInterval);
}

void example2(void) {
    std::vector<std::vector<int>> intervals{{1,2},{3,5},{6,7},{8,10},{12,16}};
    std::vector<int> newInterval{4,8};
    example(intervals, newInterval);
}

int main(int argc, char **argv) {
    example1();
    example2();
}
