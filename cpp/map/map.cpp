// SPDX-License-Identifier: MIT
// Jason Turner: https://www.youtube.com/watch?v=INn3xa4pMfg
// C++ Weekly - Ep 233 - std::map vs constexpr map (huge perf difference!)
// x86_64-pc-linux-gnu-gcc-10.2.0 -std=c++2a -O3 -lstdc++ map.cpp -o map

#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <map>

template <typename Key, typename Value, std::size_t Size>
struct Map {
    std::array<std::pair<Key, Value>, Size> data;

    [[nodiscard]] constexpr Value at(const Key &key) const {
        const auto it = std::find_if(begin(data), end(data),
                            [&key](const auto &v) { return v.first == key; });
        if (it != end(data))
            return it->second;
        else
            throw std::range_error("Not Found");
    }
};

using namespace std::literals::string_view_literals;
static constexpr std::array<std::pair<std::string_view, int>, 8> color_values
    {{{"black"sv, 7},
      {"blue"sv, 3},
      {"cyan"sv, 5},
      {"green"sv, 2},
      {"magenta"sv, 6},
      {"red"sv, 1},
      {"white"sv, 8},
      {"yellow"sv, 4}}};

int lookup_values(const std::string_view sv) {
#if 0
    static const auto map = std::map<std::string_view, int>{
        color_values.begin(), color_values.end()
    };
#else
    static constexpr auto map = Map<std::string_view, int, color_values.size()>{
        {color_values}
    };
#endif
    return map.at(sv);
}

int main()
{
    static constexpr auto map = Map<std::string_view, int, color_values.size()>{
        {color_values}
    };

    constexpr auto value = map.at("green");
    return value;
}