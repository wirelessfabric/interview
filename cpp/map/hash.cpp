// SPDX-License-Identifier: MIT
// Dietmar Kühl https://www.youtube.com/watch?v=CJsQSIp7-Ig
// #Hashing ( Hashing Values ) - [ACCU 2019]
// x86_64-pc-linux-gnu-gcc-10.2.0 -std=c++2a -O3 -lstdc++ hash.cpp -o hash

#include <vector>
#include <utility>      // std::pair
#include <algorithm>    // std::lower_bound(), std::find_if()
#include <map>
#include <unordered_map>

// linear search
std::vector<pair<K,V>> c == ...;
auto it = std::find_if(begin(c), end(c), [=](auto&& p){ return v == p.first; });
if (it == end(c)) { not-found }
else { use it->second; }

// linear search
std::vector<std::pair<K,V>> c == ...;
auto it = std::lower_bound(begin(c), end(c), [=](auto&& p){ return v == p.first; });
if (it == end(c) || it->second != k) { not-found }
else { use it->second; }

// binary search
// use a map
// implies k < x is defined
std::map<K, V> c = ...;
auto it = c.find(k);
if (it == end(c)) { not-found }
else { use it->second }

// hash table
std::unordered_map<K, V> c = ...;
auto it = c.find(k);
if (it == end(c)) { not-found }
else { use it->second }
