// SPDX-License-Identifier: MIT
// g++ -std=c++20 -O3 -lstdc++ -lm kokkos_mdspan.cpp -o kokkos_mdspan

// To try using subscript operator comment in macro below
// the header will by default also check for the feature macro, and enable it
// defining the macro to 0 will overwrite the automatic setting
// x86-64 clang (experimental auto NSDMI) supports the operator, but you need
// to explicitly comment in below macro
//#define MDSPAN_USE_BRACKET_OPERATOR 1

// To force enable operator() comment in the macro below
// You can enable both at the same time. 
//#define MDSPAN_USE_PAREN_OPERATOR 0

#include <https://raw.githubusercontent.com/kokkos/mdspan/single-header/mdspan.hpp>
#include <iostream>

namespace stdex = std::experimental;

int main() {
  std::array d{
    0, 5, 1,
    3, 8, 4,
    2, 7, 6,
  };

  stdex::mdspan m{d.data(), stdex::extents{3, 3}};

  for (std::size_t i = 0; i < m.extent(0); ++i)
    for (std::size_t j = 0; j < m.extent(0); ++j)
    #if MDSPAN_USE_BRACKET_OPERATOR
      std::cout << "m(" << i << ", " << j << ") == " << m[i, j] << "\n";
    #else
      std::cout << "m(" << i << ", " << j << ") == " << m(i, j) << "\n";
    #endif
}
