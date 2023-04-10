// SPDX-License-Identifier: MIT

#include <vector>
#include <array>
#include <random>       // std::mt19937
#include <algorithm>    // std::is_sorted(), std::shuffle()

void bogoSort(std::vector<int> &v) {
    std::mt19937 gen(std::random_device{}());
    while (!std::is_sorted(begin(v), end(v)))
        std::shuffle(begin(v), end(v), gen);
}
