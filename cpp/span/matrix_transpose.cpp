// SPDX-License-Identifier: MIT
// g++ -std=c++23 -O3 -lstdc++ -lm matrix_transpose.cpp -o matrix_transpose

// C++ Standard Parallelism - Bryce Adelstein Lelbach - CppCon 2021
// https://www.youtube.com/watch?v=KK3JXvSiJG4

// #include <https://raw.githubusercontent.com/ericniebler/range-v3/master/include/range/v3/view/cartesian_product.hpp>

#include <iostream>
#include <https://raw.githubusercontent.com/kokkos/mdspan/single-header/mdspan.hpp>
#include <span>
#include <numeric>
#include <algorithm>
#include <execution>
#include <ranges>

using std::execution::par_unseq;

namespace common::os {
    template<typename T>
    std::shared_ptr<T> make_shared(size_t n) {
        auto uptr = std::make_unique<T>(n);
        return std::shared_ptr<T>(uptr.release(), uptr.get_deleter());
    }
}

template <typename T = float>
class Matrix {
public:
    Matrix(int N, int M) :
        m_rows{ N },
        m_cols{ M },
        m_size{ (size_t)N * (size_t)M },
        m_sptr{ common::os::make_shared<float[]>(m_size) }
    {}
    auto rows() const { return m_rows; }
    auto cols() const { return m_cols; }
    auto sptr() const { return m_sptr; }
    auto data() const { return m_sptr.get(); }
    auto span() const { return std::span(data(), rows() * cols()); }
    auto span2d() const { return std::experimental::mdspan(data(), rows(), cols()); }
    auto row(int i) const { return i * cols(); }
    T& operator()(int i, int j) { return m_sptr[row(i) + j]; }
private:
    int m_rows{ 0 };
    int m_cols{ 0 };
    size_t m_size{ 0 };
    std::shared_ptr<float[]> m_sptr{ nullptr };
};

class Solution {
public:
    int N{ 640 };
    int M{ 480 };

    void matrix_transpose_span() {
        auto input{ Matrix(N, M) };
        auto output{ Matrix(M, N) };
        auto A{ input.span() };
        auto B{ output.span() };

        auto v = std::views::cartesian_product(
            std::views::iota(0, N),
            std::views::iota(0, M));

        std::for_each(par_unseq, v.begin(), v.end(),
            [=] (auto index) { 
                auto [i, j] = index;
                B[i + j * N] = A[i * M + j];
            });
    }

    void matrix_transpose_span2d() {
        auto input{ Matrix(N, M) };
        auto output{ Matrix(M, N) };
        auto A{ input.span2d() };
        auto B{ output.span2d() };

        auto v = std::views::cartesian_product(
            std::views::iota(0, A.extent(0)),
            std::views::iota(0, A.extent(1)));

        std::for_each(par_unseq, v.begin(), v.end(),
            [=] (auto index) {
                auto [i, j] = index;
                B[j, i] = A[i, j];
            });
    }

    void stencil_inner_kernel_mdspan() {
        std::mdspan A{input,  N, M, O};
        std::mdspan B{output, M, N, O};

        auto v = std::views::cartesian_product(
            std::views::iota(1, A.extent(0) - 1),
            std::views::iota(1, A.extent(1) - 1),
            std::views::iota(1, A.extent(2) - 1));

        std::for_each(par_unseq, v.begin(), v.end(),
            [=] (auto index) { 
              auto [i, j, k] = index;
              B[i, j, k] =   ( A[i,   j, k-1] +
                               A[i-1, j, k  ] +
                A[i, j-1, k] + A[i,   j, k  ] + A[i, j+1, k]
                             + A[i+1, j, k  ]
                             + A[i,   j, k+1] ) / 7.0
            });
    }
};

int main(int argc, char **argv) {
    Solution S;
    S.matrix_transpose_span();
    S.matrix_transpose_span2d();
    return 0;
}
