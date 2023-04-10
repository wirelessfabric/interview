// SPDX-License-Identifier: MIT

template<typename T>
class new_placement
{
public:
    new_placement() {
        // Construct a `T` object, placing it directly into pre-allocated
        // storage at memory address `buf`
        obj = new(buf) T;
    }

    ~new_placement() {
        // You must **manually** call the object's destructor if its
        // side effects is depended by the program
        if (obj) {
            obj->~T();
            obj = nullptr;
        }
    }

private:
    // Statically allocate the storage with automatic storage duration
    // which is large enough for any object of type `T`
    alignas(T) unsigned char buf[sizeof(T)];

public:
    T* obj{ nullptr };
}

// msvc: /O2 /W3 /std:c++17
// gcc: -O2
