// SPDX-License-Identifier: MIT

#ifndef SPEEDUP_H_
#define SPEEDUP_H_

#include <iostream>

static const auto speedup = []()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

#endif // SPEEDUP_H_
