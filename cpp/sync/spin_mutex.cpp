// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ spin_mutex.cpp -o spin_mutex

#include <atomic>
#include <thread>
#include <array>

// https://en.cppreference.com/w/cpp/atomic/atomic_flag
// Warning! This is for demo purposes. Do not use a spinlock for locking
// since it eats the time slice of your process.
// See Linus Torvalds' rant on spinlock
// https://www.realworldtech.com/forum/?threadid=189711&curpostid=189723

// __clang__
// __GNUC__
// _MSC_VER

// __i386__
// i386

// __x86_64__

// __arm__
// arm
// ARM
// _ARM_

// __aarch64__

// __ARM_NEON__

// ANDROID

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
inline void osYieldProcessor() { YieldProcessor(); }
inline void osYieldThread() { SwitchToThread(); }
#else
#if defined(__x86_64__) || defined(__i386__)
#include <emmintrin.h>
inline void osYieldProcessor() { _mm_pause(); }
#elif defined(__aarch64__) || defined(__arm__)
inline void osYieldProcessor() { __asm__ __volatile__("yield"); }
#else
inline void osYieldProcessor() {}
#warning "Processor yield not supported on this platform"
#endif
#if defined(__cplusplus)
inline void osYieldThread() { std::this_thread::yield(); }
#elif defined(POSIX)
inline void osYieldThread() { sched_yield(); }
#else
inline void osYieldThread() {}
#warning "Thread yield not supported on this platform"
#endif
#endif

namespace portable {
    struct spin_mutex {
        void lock() noexcept {
            constexpr std::array iterations = { 5, 10, 3000 };

            for (auto i = 0; i < iterations[0]; ++i)
                if (try_lock())
                    return;

            for (auto i = 0; i < iterations[1]; ++i) {
                if (try_lock())
                    return;

                osYieldProcessor();
            }

            while (true) {
                for (auto i = 0; i < iterations[2]; ++i) {
                    if (try_lock())
                        return;

                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                    osYieldProcessor();
                }

                osYieldThread();
            }
        }

        bool try_lock() noexcept {
            return !flag.test_and_set(std::memory_order_acquire);
        }

        void unlock() noexcept {
            flag.clear(std::memory_order_release);
            flag.notify_one();
        }

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
}

// Timur Doumler - Using Locks in Real Time Audio Processing, Safely 
// https://www.youtube.com/watch?v=DHx9kv74Mlc
// @ 48:23 

struct spin_mutex {
    void lock() noexcept {
        constexpr std::array iterations = { 5, 10, 3000 };

        for (auto i = 0; i < iterations[0]; ++i)
            if (try_lock())
                return;

        for (auto i = 0; i < iterations[1]; ++i) {
            if (try_lock())
                return;

            _mm_pause();
        }

        while (true) {
            for (auto i = 0; i < iterations[2]; ++i) {
                if (try_lock())
                    return;

                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
            }

            std::this_thread::yield();
        }
    }

    bool try_lock() noexcept {
        return !flag.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept {
        flag.clear(std::memory_order_release);
        flag.notify_one()
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Timur Doumler - Using Locks in Real Time Audio Processing, Safely 
// https://www.youtube.com/watch?v=DHx9kv74Mlc
// @ 32:12 Intrinsics

struct spin_mutex {
    void lock() noexcept {
        for (std::uint64_t k = 0; !try_lock(); ++k) {
            if (k < 4) { /* spin */ }
            else if (k < 16) { __mm_pause(); }
            else if (k < 64) { std::this_thread::yield(); }
            else {
                timespec rqtp = {0, 0};
                rqtp.tv_sec = 0;
                rqtp.tv_nsec = 1000;
                nanosleep(&rqtp, nullptr);
            }
        }
    }

    bool try_lock() noexcept {
        return !flag.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept {
        flag.clear(std::memory_order_release);
        flag.notify_one()
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// The C++20 Synchronization Library - Bryce Adelstein Lelbach - CppCon 2019
// https://www.youtube.com/watch?v=Zcqwb3CWqs4
// @ 30:20

struct spin_mutex {
    void lock() noexcept {
        while (flag.test_and_set(std::memory_order_acquire))
            flag.wait(true, std::memory_order_relaxed);
    }

    void unlock() noexcept {
        flag.clear(std::memory_order_release);
        flag.notify_one()
    }

    bool try_lock() noexcept {
        return !flag.test_and_set(std::memory_order_acquire);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Timur Doumler - Using Locks in Real Time Audio Processing, Safely 
// https://www.youtube.com/watch?v=DHx9kv74Mlc
// @ 21:35 Naive

struct spin_mutex {
    void lock() noexcept { while (!try_lock()) { /* spin */ } }
    void unlock() noexcept { flag.clear(std::memory_order_release); }
    bool try_lock() noexcept { return !flag.test_and_set(std::memory_order_acquire); }
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part I: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI
// @ 44:40

struct spin_mutex {
    void lock() noexcept { while (flag.test_and_set()) { /* spin */ } }
    void unlock() noexcept { flag.clear(); }
    bool try_lock() noexcept { return !flag.test_and_set(); }
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Timur Doumler - Using Locks in Real Time Audio Processing, Safely 
// https://www.youtube.com/watch?v=DHx9kv74Mlc
// @ 26:45 Exponential Back-Off

struct spin_mutex {
    void lock() noexcept {
        for (std::uint64_t k = 0; !try_lock(); ++k) {
            if (k < 4) { /* spin */ }
            else if (k < 16) { __asm__ __volatile__("rep; nop" : : : "memory"); }
            else if (k < 64) { sched_yield(); }
            else {
                timespec rqtp = {0, 0};
                rqtp.tv_sec = 0;
                rqtp.tv_nsec = 1000;
                nanosleep(&rqtp, nullptr);
            }
        }
    }

    bool try_lock() noexcept {
        return !flag.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Chromium SpinLock
// https://chromium.googlesource.com/chromium/src/third_party/WebKit/Source/wtf/+/823d62cdecdbd5f161634177e130e5ac01eb7b48/SpinLock.cpp

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "wtf/SpinLock.h"
#include "wtf/Atomics.h"
#include "wtf/CPU.h"
#include "wtf/Compiler.h"
#if OS(WIN)
#include <windows.h>
#elif OS(POSIX)
#include <sched.h>
#endif
// The YIELD_PROCESSOR macro wraps an architecture specific-instruction that
// informs the processor we're in a busy wait, so it can handle the branch more
// intelligently and e.g. reduce power to our core or give more resources to the
// other hyper-thread on this core. See the following for context:
// https://software.intel.com/en-us/articles/benefitting-power-and-performance-sleep-loops
//
// The YIELD_THREAD macro tells the OS to relinquish our quanta. This is
// basically a worst-case fallback, and if you're hitting it with any frequency
// you really should be using proper lock rather than these spinlocks.
#if OS(WIN)
#define YIELD_PROCESSOR YieldProcessor()
#define YIELD_THREAD SwitchToThread()
#elif COMPILER(GCC) || COMPILER(CLANG)
#if CPU(X86_64) || CPU(X86)
#define YIELD_PROCESSOR __asm__ __volatile__("pause")
#elif CPU(ARM) || CPU(ARM64)
#define YIELD_PROCESSOR __asm__ __volatile__("yield")
#elif CPU(MIPS)
// The MIPS32 docs state that the PAUSE instruction is a no-op on older
// architectures (first added in MIPS32r2). To avoid assembler errors when
// targeting pre-r2, we must encode the instruction manually.
#define YIELD_PROCESSOR __asm__ __volatile__(".word 0x00000140")
#elif CPU(MIPS64) && __mips_isa_rev >= 2
// Don't bother doing using .word here since r2 is the lowest supported mips64
// that Chromium supports.
#define YIELD_PROCESSOR __asm__ __volatile__("pause")
#endif
#endif
#ifndef YIELD_PROCESSOR
#warning "Processor yield not supported on this architecture."
#define YIELD_PROCESSOR ((void)0)
#endif
#ifndef YIELD_THREAD
#if OS(POSIX)
#define YIELD_THREAD sched_yield()
#else
#warning "Thread yield not supported on this OS."
#define YIELD_THREAD ((void)0)
#endif
#endif
namespace WTF {
void SpinLock::lockSlow()
{
    // The value of kYieldProcessorTries is cargo culted from TCMalloc, Windows
    // critical section defaults, and various other recommendations.
    // TODO(jschuh): Further tuning may be warranted.
    static const int kYieldProcessorTries = 1000;
    do {
        do {
            for (int count = 0; count < kYieldProcessorTries; ++count) {
                // Let the Processor know we're spinning.
                YIELD_PROCESSOR;
                if (!m_lock.load(std::memory_order_relaxed) && LIKELY(!m_lock.exchange(true, std::memory_order_acquire)))
                    return;
            }
            // Give the OS a chance to schedule something on this core.
            YIELD_THREAD;
        } while (m_lock.load(std::memory_order_relaxed));
    } while (UNLIKELY(m_lock.exchange(true, std::memory_order_acquire)));
}
} // namespace WTF
