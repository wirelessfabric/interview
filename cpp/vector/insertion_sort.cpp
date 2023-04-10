// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 insertion_sort.cpp -o insertion_sort -lm -ltbb
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++17 insertion_sort.cpp
// cl version 19.35.32215 for x64
//
// See: https://en.cppreference.com/w/cpp/algorithm/rotate

#include <algorithm>
#include <iostream>
#include <vector>
 
auto print = [](auto const remark, auto const& v)
{
    std::cout << remark;
    for (auto n : v)
        std::cout << n << ' ';
    std::cout << '\n';
};
 
int main()
{
    std::vector<int> v{2, 4, 2, 0, 5, 10, 7, 3, 7, 1};
    print("before sort:\t\t", v);
 
    // insertion sort
    for (auto i = v.begin(); i != v.end(); ++i)
        std::rotate(std::upper_bound(v.begin(), i, *i), i, i + 1);
    print("after sort:\t\t", v);
}
