// SPDX-License-Identifier: MIT

#include <vector>
#include <array>
#include <utility>      // std::pair
#include <algorithm>    // std::find_if()

std::vector<pair<K,V>> data;
auto it = std::find_if(begin(data), end(data), [=](auto&& pair){ return pair.first == key; });

std::array<std::pair<K,V>, Size> data;
const auto it = std::find_if(begin(data), end(data), [&key](const auto &pair){ return pair.first == key; });

std::vector<int> data{1, 2, 3, 4};
auto it = std::find_if(begin(data), end(data), [](auto value){ return value % 2 == 0; });
