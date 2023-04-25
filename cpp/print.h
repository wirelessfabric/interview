// SPDX-License-Identifier: MIT

#ifndef _PRINT_H_
#define _PRINT_H_

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>

void print_begin(const char* text = nullptr) {
    if (text)
        std::cout << text << " = ";
    std::cout << "[";
}

void print_end(const char* text = nullptr) {
    std::cout << "]";
    if (text)
        std::cout << std::endl;
}

template<typename T>
void print(T *v, int n, const char* text = nullptr) {
    assert(v && n > 0);
    print_begin(text);
    do {
        std::cout << *v++;
        if (--n)
            std::cout << ", ";
    } while (n);
    print_end(text);
}

template<typename T>
void print(const std::vector<T>& v, const char* text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto& e : v) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template<typename T> 
void print(const std::vector<T>& v, int start, int end, const char* text = nullptr) {
    print_begin(text);
    int n = 0;
    for (auto i=start; i <= end; ++i) {
        if (n++)
            std::cout << ",";
        std::cout << v[i];
    }
    print_end(text);
}

template<typename T>
void print(const std::vector<T>& v, int start, int end) {
    std::cout << "[";
    int n = 0;
    for (auto i=start; i <= end; ++i) {
        if (n++)
            std::cout << ",";
        std::cout << v[i];
    }
    std::cout << "]" << std::endl;
}

template<typename T> 
void print(const std::vector<std::vector<T>>& vv, const char* text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto& v : vv) {
        if (n++)
            std::cout << ",";
        print(v);
    }
    print_end(text);
}

template<typename T, size_t N>
void print(const std::array<T, N>& a, const char* text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto& e : a) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template<typename K, typename V>
void print(const std::map<K,V>& m, const char* text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto& it : m) {
        if (n++)
            std::cout << ",";
        std::cout << "[" << it.first << "," << it.second << "]";
    }
    print_end(text);
}

void print(const std::unordered_map<int, int>& map) {
    for (const auto &[key, value] : map)
        std::cout << key << " = " << value << std::endl;
}

#endif // _PRINT_H_
