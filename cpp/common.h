// SPDX-License-Identifier: MIT

#ifndef COMMON_H_
#define COMMON_H_

#if !defined(__GNUC__) || __GNUC__ < 9
#define CPP_STD_EXECUTION
#define CPP_STD_NUMBERS
#endif

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <tuple>
#include <numeric>
#include <functional>
#include <algorithm>
#ifdef CPP_STD_EXECUTION
#include <execution>
#endif
#ifdef CPP_STD_NUMBERS
#include <numbers>
#endif
#include <random>
#include <chrono>
#include <limits>
#include <climits>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <cmath>
#include <ctime>
#include <iomanip>

#endif // COMMON_H_
