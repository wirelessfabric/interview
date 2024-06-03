// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -o fifo fifo.cpp

#include <atomic>
#include <vector>
#include <functional>

// Fabian Renn-Giles & Dave Rowland @ ADC19
// Real-time 101 - part II: Investigating the real-time problem space
// https://www.youtube.com/watch?v=Q0vrQFyAdWI

// @ 17:55 - Single Producer, Single Consumer FIFO

template <typename T>
class fifo {
public:
    bool push(T&& arg) {
        auto pos = writepos.load();
        auto next = (pos + 1) % slots.size();
        if (next == readpos.load())
            return false;
        
        slots[pos] = std::move(arg);
        writepos.store(next);
        return true;
    }

    bool pop(T& result) {
        auto pos = readpos.load();
        if (pos == writepos.load())
            return false;
        
        result = std::move(slots[pos]);
        readpos.store((pos + 1) % slots.size());
        return true;
    }

private:
    std::vector<T> slots = {};
    std::atomic<int> readpos{ 0 };
    std::atomic<int> writepos{ 0 };
};

// @ 25:20 - async caller

class async_caller {
public:
    void call_async(std::function<void()>&& lambda) {
        auto success = queue.push(std::move(lambda));
        assert(success);
    }

    void process() {
        std::function<void()> lambda;
        while (queue.pop(lambda))
            lambda();
    }

private:
    fifo<std::function<void()>> queue;
};

async_caller executor;

void timer_callback() {
    executor.process();
}

void realtime_thread() {
    while (running) {
        // do work
        executor.call_async([](){ std::cout << "Hello World!" << std::endl; });
        // do more work
    }
}

// Charles Frasch @ CppCon 2023
// Single Producer Single Consumer Lock-free FIFO From the Ground Up
// https://www.youtube.com/watch?v=K3P_Lmq6pw0
// https://github.com/CharlesFrasch/cppcon2023.git

// https://www.boost.org/doc/libs/1_82_0/boost/lockfree/spsc_queue.hpp
// https://github.com/rigtorp/SPSCQueue
// https://www.1024cores.net/home/lock-free-algorithms/queues/unbounded-spsc-queue
// https://www.dpdk.org/

// @ 10:36 - Single Producer, Single Consumer FIFO

#pragma once

#include <cassert>
#include <cstdlib>
#include <memory>

/// Non-threadsafe circular FIFO; has data races
template<typename T, typename Alloc = std::allocator<T>>
class Fifo1 : private Alloc
{
public:
    using value_type = T;
    using allocator_traits = std::allocator_traits<Alloc>;
    using size_type = typename allocator_traits::size_type;

    explicit Fifo1(size_type capacity, Alloc const& alloc = Alloc{})
        : Alloc{alloc}
        , capacity_{capacity}
        , ring_{allocator_traits::allocate(*this, capacity)}
    {}

    // For consistency with other fifos
    Fifo1(Fifo1 const&) = delete;
    Fifo1& operator=(Fifo1 const&) = delete;
    Fifo1(Fifo1&&) = delete;
    Fifo1& operator=(Fifo1&&) = delete;

    ~Fifo1() {
        while(not empty()) {
            ring_[popCursor_ % capacity_].~T();
            ++popCursor_;
        }
        allocator_traits::deallocate(*this, ring_, capacity_);
    }


    /// Returns the number of elements in the fifo
    auto size() const noexcept {
        assert(popCursor_ <= pushCursor_);
        return pushCursor_ - popCursor_;
    }

    /// Returns whether the container has no elements
    auto empty() const noexcept { return size() == 0; }

    /// Returns whether the container has capacity_() elements
    auto full() const noexcept { return size() == capacity(); }

    /// Returns the number of elements that can be held in the fifo
    auto capacity() const noexcept { return capacity_; }


    /// Push one object onto the fifo.
    /// @return `true` if the operation is successful; `false` if fifo is full.
    auto push(T const& value) {
        if (full()) {
            return false;
        }
        new (&ring_[pushCursor_ % capacity_]) T(value);
        ++pushCursor_;
        return true;
    }

    /// Pop one object from the fifo.
    /// @return `true` if the pop operation is successful; `false` if fifo is empty.
    auto pop(T& value) {
        if (empty()) {
            return false;
        }
        value = ring_[popCursor_ % capacity_];
        ring_[popCursor_ % capacity_].~T();
        ++popCursor_;
        return true;
    }

private:
    size_type capacity_;
    T* ring_;
    size_type pushCursor_{};
    size_type popCursor_{};
};

// @ 21:10 - Naive

#pragma once

#include <atomic>
#include <cassert>
#include <memory>

/// Threadsafe but flawed circular FIFO
template<typename T, typename Alloc = std::allocator<T>>
class Fifo2 : private Alloc
{
public:
    using value_type = T;
    using allocator_traits = std::allocator_traits<Alloc>;
    using size_type = typename allocator_traits::size_type;

    explicit Fifo2(size_type capacity, Alloc const& alloc = Alloc{})
        : Alloc{alloc}
        , capacity_{capacity}
        , ring_{allocator_traits::allocate(*this, capacity)}
    {}

    ~Fifo2() {
        while(not empty()) {
            ring_[popCursor_ % capacity_].~T();
            ++popCursor_;
        }
        allocator_traits::deallocate(*this, ring_, capacity_);
    }

    /// Returns the number of elements in the fifo
    auto size() const noexcept {
        assert(popCursor_ <= pushCursor_);
        return pushCursor_ - popCursor_;
    }

    /// Returns whether the container has no elements
    auto empty() const noexcept { return size() == 0; }

    /// Returns whether the container has capacity_() elements
    auto full() const noexcept { return size() == capacity(); }

    /// Returns the number of elements that can be held in the fifo
    auto capacity() const noexcept { return capacity_; }



    /// Push one object onto the fifo.
    /// @return `true` if the operation is successful; `false` if fifo is full.
    auto push(T const& value) {
        if (full()) {
            return false;
        }
        new (&ring_[pushCursor_ % capacity_]) T(value);
        ++pushCursor_;
        return true;
    }

    /// Pop one object from the fifo.
    /// @return `true` if the pop operation is successful; `false` if fifo is empty.
    auto pop(T& value) {
        if (empty()) {
            return false;
        }
        value = ring_[popCursor_ % capacity_];
        ring_[popCursor_ % capacity_].~T();
        ++popCursor_;
        return true;
    }

private:
    size_type capacity_;
    T* ring_;

    using CursorType = std::atomic<size_type>;
    static_assert(CursorType::is_always_lock_free);

    /// Loaded and stored by the push thread; loaded by the pop thread
    CursorType pushCursor_;

    /// Loaded and stored by the pop thread; loaded by the push thread
    CursorType popCursor_;
};

// @ 28:44 - Relaxed atomics, no false sharing

#pragma once

#include <atomic>
#include <cassert>
#include <memory>
#include <new>

/// Threadsafe, efficient circular FIFO
template<typename T, typename Alloc = std::allocator<T>>
class Fifo3 : private Alloc
{
public:
    using value_type = T;
    using allocator_traits = std::allocator_traits<Alloc>;
    using size_type = typename allocator_traits::size_type;

    explicit Fifo3(size_type capacity, Alloc const& alloc = Alloc{})
        : Alloc{alloc}
        , capacity_{capacity}
        , ring_{allocator_traits::allocate(*this, capacity)}
    {}

    ~Fifo3() {
        while(not empty()) {
            element(popCursor_)->~T();
            ++popCursor_;
        }
        allocator_traits::deallocate(*this, ring_, capacity_);
    }


    /// Returns the number of elements in the fifo
    auto size() const noexcept {
        auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
        auto popCursor = popCursor_.load(std::memory_order_relaxed);

        assert(popCursor <= pushCursor);
        return pushCursor - popCursor;
    }

    /// Returns whether the container has no elements
    auto empty() const noexcept { return size() == 0; }

    /// Returns whether the container has capacity_() elements
    auto full() const noexcept { return size() == capacity(); }

    /// Returns the number of elements that can be held in the fifo
    auto capacity() const noexcept { return capacity_; }


    /// Push one object onto the fifo.
    /// @return `true` if the operation is successful; `false` if fifo is full.
    auto push(T const& value) {
        auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
        auto popCursor = popCursor_.load(std::memory_order_acquire);
        if (full(pushCursor, popCursor)) {
            return false;
        }
        new (element(pushCursor)) T(value);
        pushCursor_.store(pushCursor + 1, std::memory_order_release);
        return true;
    }

    /// Pop one object from the fifo.
    /// @return `true` if the pop operation is successful; `false` if fifo is empty.
    auto pop(T& value) {
        auto pushCursor = pushCursor_.load(std::memory_order_acquire);
        auto popCursor = popCursor_.load(std::memory_order_relaxed);
        if (empty(pushCursor, popCursor)) {
            return false;
        }
        value = *element(popCursor);
        element(popCursor)->~T();
        popCursor_.store(popCursor + 1, std::memory_order_release);
        return true;
    }

private:
    auto full(size_type pushCursor, size_type popCursor) const noexcept {
        return (pushCursor - popCursor) == capacity_;
    }
    static auto empty(size_type pushCursor, size_type popCursor) noexcept {
        return pushCursor == popCursor;
    }
    auto element(size_type cursor) noexcept {
        return &ring_[cursor % capacity_];
    }

private:
    size_type capacity_;
    T* ring_;

    using CursorType = std::atomic<size_type>;
    static_assert(CursorType::is_always_lock_free);

    // N.B. std::hardware_destructive_interference_size is not used directly
    // error: use of ‘std::hardware_destructive_interference_size’ [-Werror=interference-size]
    // note: its value can vary between compiler versions or with different ‘-mtune’ or ‘-mcpu’ flags
    // note: if this use is part of a public ABI, change it to instead use a constant variable you define
    // note: the default value for the current CPU tuning is 64 bytes
    // note: you can stabilize this value with ‘--param hardware_destructive_interference_size=64’, or disable this warning with ‘-Wno-interference-size’
    static constexpr auto hardware_destructive_interference_size = size_type{64};

    /// Loaded and stored by the push thread; loaded by the pop thread
    alignas(hardware_destructive_interference_size) CursorType pushCursor_;

    /// Loaded and stored by the pop thread; loaded by the push thread
    alignas(hardware_destructive_interference_size) CursorType popCursor_;

    // Padding to avoid false sharing with adjacent objects
    char padding_[hardware_destructive_interference_size - sizeof(size_type)];
};

// @ 34:49 - Cached cursors

#pragma once

#include <atomic>
#include <cassert>
#include <memory>
#include <new>

#include <sanitizer/tsan_interface.h>


/// Threadsafe, efficient circular FIFO with cached cursors
template<typename T, typename Alloc = std::allocator<T>>
class Fifo4 : private Alloc
{
public:
    using value_type = T;
    using allocator_traits = std::allocator_traits<Alloc>;
    using size_type = typename allocator_traits::size_type;

    explicit Fifo4(size_type capacity, Alloc const& alloc = Alloc{})
        : Alloc{alloc}
        , capacity_{capacity}
        , ring_{allocator_traits::allocate(*this, capacity)}
    {}

    ~Fifo4() {
        while(not empty()) {
            element(popCursor_)->~T();
            ++popCursor_;
        }
        allocator_traits::deallocate(*this, ring_, capacity_);
    }

    /// Returns the number of elements in the fifo
    auto size() const noexcept {
        auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
        auto popCursor = popCursor_.load(std::memory_order_relaxed);

        assert(popCursor <= pushCursor);
        return pushCursor - popCursor;
    }

    /// Returns whether the container has no elements
    auto empty() const noexcept { return size() == 0; }

    /// Returns whether the container has capacity_() elements
    auto full() const noexcept { return size() == capacity(); }

    /// Returns the number of elements that can be held in the fifo
    auto capacity() const noexcept { return capacity_; }


    /// Push one object onto the fifo.
    /// @return `true` if the operation is successful; `false` if fifo is full.
    auto push(T const& value) {
        auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
        if (full(pushCursor, popCursorCached_)) {
            popCursorCached_ = popCursor_.load(std::memory_order_acquire);
            // popCursorCached_ = popCursor_.load(std::memory_order_relaxed);
            if (full(pushCursor, popCursorCached_)) {
                return false;
            }
        }

        //__tsan_acquire(&popCursor_);
        // std::atomic_thread_fence(std::memory_order_acquire);
        new (element(pushCursor)) T(value);
        pushCursor_.store(pushCursor + 1, std::memory_order_release);
        return true;
    }

    /// Pop one object from the fifo.
    /// @return `true` if the pop operation is successful; `false` if fifo is empty.
    auto pop(T& value) {
        auto popCursor = popCursor_.load(std::memory_order_relaxed);
        if (empty(pushCursorCached_, popCursor)) {
            pushCursorCached_ = pushCursor_.load(std::memory_order_acquire);
            // pushCursorCached_ = pushCursor_.load(std::memory_order_relaxed);
            if (empty(pushCursorCached_, popCursor)) {
                return false;
            }
        }

        // __tsan_acquire(&pushCursor_);
        // std::atomic_thread_fence(std::memory_order_acquire);
        value = *element(popCursor);
        element(popCursor)->~T();
        popCursor_.store(popCursor + 1, std::memory_order_release);
        return true;
    }

private:
    auto full(size_type pushCursor, size_type popCursor) const noexcept {
        return (pushCursor - popCursor) == capacity_;
    }
    static auto empty(size_type pushCursor, size_type popCursor) noexcept {
        return pushCursor == popCursor;
    }
    auto element(size_type cursor) noexcept {
        return &ring_[cursor % capacity_];
    }

private:
    size_type capacity_;
    T* ring_;

    using CursorType = std::atomic<size_type>;
    static_assert(CursorType::is_always_lock_free);

    // See Fifo3 for reason std::hardware_destructive_interference_size is not used directly
    static constexpr auto hardware_destructive_interference_size = size_type{64};

    /// Loaded and stored by the push thread; loaded by the pop thread
    alignas(hardware_destructive_interference_size) CursorType pushCursor_;

    /// Exclusive to the push thread
    alignas(hardware_destructive_interference_size) size_type popCursorCached_{};

    /// Loaded and stored by the pop thread; loaded by the push thread
    alignas(hardware_destructive_interference_size) CursorType popCursor_;

    /// Exclusive to the pop thread
    alignas(hardware_destructive_interference_size) size_type pushCursorCached_{};

    // Padding to avoid false sharing with adjacent objects
    char padding_[hardware_destructive_interference_size - sizeof(size_type)];
};

// @ 43:56 - Proxies

#pragma once

#include <atomic>
#include <cassert>
#include <cstring>
#include <memory>
#include <new>
#include <type_traits>

/// A trait used to optimize the number of bytes copied. Specialize this
/// on the type used to parameterize the Fifo5 to implement the
/// optimization. The general template returns `sizeof(T)`.
template<typename T>
struct ValueSizeTraits
{
    using value_type = T;
    static std::size_t size(value_type const& value) { return sizeof(value_type); }
};


/// Require trivial, add ValueSizeTraits, pusher and popper to Fifo4
template<typename T, typename Alloc = std::allocator<T>>
    requires std::is_trivial_v<T>
class Fifo5 : private Alloc
{
public:
    using value_type = T;
    using allocator_traits = std::allocator_traits<Alloc>;
    using size_type = typename allocator_traits::size_type;

    explicit Fifo5(size_type capacity, Alloc const& alloc = Alloc{})
        : Alloc{alloc}
        , capacity_{capacity}
        , ring_{allocator_traits::allocate(*this, capacity)}
    {}

    ~Fifo5() {
        allocator_traits::deallocate(*this, ring_, capacity());
    }


    /// Returns the number of elements in the fifo
    auto size() const noexcept {
        auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
        auto popCursor = popCursor_.load(std::memory_order_relaxed);

        assert(popCursor <= pushCursor);
        return pushCursor - popCursor;
    }

    /// Returns whether the container has no elements
    auto empty() const noexcept { return size() == 0; }

    /// Returns whether the container has capacity_() elements
    auto full() const noexcept { return size() == capacity(); }

    /// Returns the number of elements that can be held in the fifo
    auto capacity() const noexcept { return capacity_; }


    /// An RAII proxy object returned by push(). Allows the caller to
    /// manipulate value_type's members directly in the fifo's ring. The
    /// actual push happens when the pusher goes out of scope.
    class pusher_t
    {
    public:
        pusher_t() = default;
        explicit pusher_t(Fifo5* fifo, size_type cursor) noexcept : fifo_{fifo}, cursor_{cursor} {}

        pusher_t(pusher_t const&) = delete;
        pusher_t& operator=(pusher_t const&) = delete;

        pusher_t(pusher_t&& other) noexcept
            : fifo_{std::move(other.fifo_)}
            , cursor_{std::move(other.cursor_)} {
            other.release();
        }
        pusher_t& operator=(pusher_t&& other) noexcept {
            fifo_ = std::move(other.fifo_);
            cursor_ = std::move(other.cursor_);
            other.release();
            return *this;
        }

        ~pusher_t() {
            if (fifo_) {
                fifo_->pushCursor_.store(cursor_ + 1, std::memory_order_release);
            }
        }

        /// If called the actual push operation will not be called when the
        /// pusher_t goes out of scope. Operations on the pusher_t instance
        /// after release has been called are undefined.
        void release() noexcept { fifo_ = {}; }

        /// Return whether or not the pusher_t is active.
        explicit operator bool() const noexcept { return fifo_; }

        /// @name Direct access to the fifo's ring
        ///@{
        value_type* get() noexcept { return fifo_->element(cursor_); }
        value_type const* get() const noexcept { return fifo_->element(cursor_); }

        value_type& operator*() noexcept { return *get(); }
        value_type const& operator*() const noexcept { return *get(); }

        value_type* operator->() noexcept { return get(); }
        value_type const* operator->() const noexcept { return get(); }
        ///@}

        /// Copy-assign a `value_type` to the pusher. Prefer to use this
        /// form rather than assigning directly to a value_type&. It takes
        /// advantage of ValueSizeTraits.
        pusher_t& operator=(value_type const& value) noexcept {
            std::memcpy(get(), std::addressof(value), ValueSizeTraits<value_type>::size(value));
            return *this;
        }

    private:
        Fifo5* fifo_{};
        size_type cursor_;
    };
    friend class pusher_t;

    /// Optionally push one object onto a file via a pusher.
    /// @return a pointer to pusher_t.
    pusher_t push() noexcept {
        auto pushCursor = pushCursor_.load(std::memory_order_relaxed);
        if (full(pushCursor, popCursorCached_)) {
            popCursorCached_ = popCursor_.load(std::memory_order_acquire);
            if (full(pushCursor, popCursorCached_)) {
                return pusher_t{};
            }
        }
        return pusher_t(this, pushCursor);
    }

    /// Push one object onto the fifo.
    /// @return `true` if the operation is successful; `false` if fifo is full.
    auto push(T const& value) noexcept {
        auto pusher = push();
        if (pusher) {
            pusher = value;
            return true;
        }
        return false;
    }

    /// An RAII proxy object returned by pop(). Allows the caller to
    /// manipulate value_type members directly in the fifo's ring. The
    // /actual pop happens when the popper goes out of scope.
    class popper_t
    {
    public:
        popper_t() = default;
        explicit popper_t(Fifo5* fifo, size_type cursor) noexcept : fifo_{fifo}, cursor_{cursor} {}

        popper_t(popper_t const&) = delete;
        popper_t& operator=(popper_t const&) = delete;

        popper_t(popper_t&& other) noexcept
            : fifo_{std::move(other.fifo_)}
            , cursor_{std::move(other.cursor_)} {
            other.release();
        }
        popper_t& operator=(popper_t&& other) noexcept {
            fifo_ = std::move(other.fifo_);
            cursor_ = std::move(other.cursor_);
            other.release();
            return *this;
        }

        ~popper_t() {
            if (fifo_) {
                fifo_->popCursor_.store(cursor_ + 1, std::memory_order_release);
            }
        }

        /// If called the actual pop operation will not be called when the
        /// popper_t goes out of scope. Operations on the popper_t instance
        /// after release has been called are undefined.
        void release() noexcept { fifo_ = {}; }

        /// Return whether or not the popper_t is active.
        explicit operator bool() const noexcept { return fifo_; }

        /// @name Direct access to the fifo's ring
        ///@{
        value_type* get() noexcept { return fifo_->element(cursor_); }
        value_type const* get() const noexcept { return fifo_->element(cursor_); }

        value_type& operator*() noexcept { return *get(); }
        value_type const& operator*() const noexcept { return *get(); }

        value_type* operator->() noexcept { return get(); }
        value_type const* operator->() const noexcept { return get(); }
        ///@}

    private:
        Fifo5* fifo_{};
        size_type cursor_;
    };
    friend popper_t;

    auto pop() noexcept {
        auto popCursor = popCursor_.load(std::memory_order_relaxed);
        if (empty(pushCursorCached_, popCursor)) {
            pushCursorCached_ = pushCursor_.load(std::memory_order_acquire);
            if (empty(pushCursorCached_, popCursor)) {
                return popper_t{};
            }
        }
        return popper_t(this, popCursor);
    };

    /// Pop one object from the fifo.
    /// @return `true` if the pop operation is successful; `false` if fifo is empty.
    auto pop(T& value) noexcept {
        auto popper = pop();
        if (popper) {
            value = *popper;
            return true;
        }
        return false;
    }

private:
    auto full(size_type pushCursor, size_type popCursor) const noexcept {
        assert(popCursor <= pushCursor);
        return (pushCursor - popCursor) == capacity();
    }
    static auto empty(size_type pushCursor, size_type popCursor) noexcept {
        return pushCursor == popCursor;
    }

    auto* element(size_type cursor) noexcept { return &ring_[cursor % capacity_]; }
    auto const* element(size_type cursor) const noexcept { return &ring_[cursor % capacity_]; }

private:
    size_type capacity_;
    T* ring_;

    using CursorType = std::atomic<size_type>;
    static_assert(CursorType::is_always_lock_free);

    // https://stackoverflow.com/questions/39680206/understanding-stdhardware-destructive-interference-size-and-stdhardware-cons
    // See Fifo3.hpp for reason why std::hardware_destructive_interference_size is not used directly
    static constexpr auto hardware_destructive_interference_size = size_type{64};

    /// Loaded and stored by the push thread; loaded by the pop thread
    alignas(hardware_destructive_interference_size) CursorType pushCursor_;

    /// Exclusive to the push thread
    alignas(hardware_destructive_interference_size) size_type popCursorCached_{};

    /// Loaded and stored by the pop thread; loaded by the push thread
    alignas(hardware_destructive_interference_size) CursorType popCursor_;

    /// Exclusive to the pop thread
    alignas(hardware_destructive_interference_size) size_type pushCursorCached_{};

    // Padding to avoid false sharing with adjacent objects
    char padding_[hardware_destructive_interference_size - sizeof(size_type)];
};
