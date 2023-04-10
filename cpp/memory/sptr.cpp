// SPDX-License-Identifier: MIT

#include <memory>

namespace portable {
    template<typename T>
    std::shared_ptr<T> make_shared(size_t n) {
        auto uptr = std::make_unique<T>(n);
        return std::shared_ptr<T>(uptr.release(), uptr.get_deleter());
    }
}

struct example {
    std::shared_ptr<float[]> sptr{ nullptr };

    void allocate_buffer(int rows, int cols) {
        auto n_pixels{ (size_t)rows * (size_t)cols };
        sptr = portable::make_shared<float[]>(n_pixels);
    }

    void set_at(int index, float value) {
        auto p = sptr.get();
        p[index] = value;
    }
};

example object;

int main() {
    object.allocate_buffer(640, 480);

    return 0;
}

int thread_fn() {
    object.set_at(0, 5.f);

    return 0;
}


// msvc: /O2 /W3 /std:c++17
// gcc: -O2
