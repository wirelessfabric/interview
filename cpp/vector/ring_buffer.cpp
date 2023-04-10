// SPDX-License-Identifier: MIT

#include <vector>

template<class T>
class RingBuffer {
public:
    explicit RingBuffer(std::size_t size) :
        buffer{ new char[size] },
        buffer_size{ size },
        head{ 0 },
        tail{ 0 },
        count{ 0 }
    {}

    ~RingBuffer() {
        delete[] buffer;
    }

    T get() {
        if (tail == size)
            tail = 0;

        count--;
        return buffer[tail++];
    }

    void put(T value) {
        if (head == size)
            head = 0;

        count++;
        buffer[head++] = value;
    }

    bool full() { return count == buffer_size; }
    bool empty() { return count == 0; }

    std::size_t size() { return buffer_size; }

private:
    T *buffer{ nullptr };
    std::size_t buffer_size{ 0 };
    std::size_t head{ 0 };
    std::size_t tail{ 0 };
    std::size_t count{ 0 };
};

