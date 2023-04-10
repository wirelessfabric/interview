// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -o fifo fifo.cpp

#include <atomic>
#include <vector>
#include <functional>

// Fabian Renn-Giles & Dave Rowland - Real-time 101 - part II: Investigating the real-time problem space
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
