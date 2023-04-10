// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ ticket_mutex.cpp -o ticket_mutex

#include <atomic>
#include <new>

// The C++20 Synchronization Library - Bryce Adelstein Lelbach - CppCon 2019
// https://www.youtube.com/watch?v=Zcqwb3CWqs4
// @ 32:31

struct ticket_mutex {
    void lock() noexcept {
        auto const mine = in.fetch_add(1, std::memory_order_acquire);
        while (true) {
            const auto now = out.load(std::memory_order_acquire);
            if (now == mine)
                return;
            out.wait(now, std::memory_order_relaxed);
        }
    }

    void unlock() noexcept {
        out.fetch_add(1, std::memory_order_release);
        out.notify_all();
    }

    bool try_lock() noexcept {
        // TBD
    }

private:
    alignas(std::hardware_destructive_interference_size) std::atomic<int> in = ATOMIC_VAR_INIT(0);
    alignas(std::hardware_destructive_interference_size) std::atomic<int> out = ATOMIC_VAR_INIT(0);
};
