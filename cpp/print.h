// SPDX-License-Identifier: MIT

#ifndef PRINT_H_
#define PRINT_H_

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>

static void print_begin(const char *text = nullptr)
{
    if (text)
        std::cout << text << " = ";
    std::cout << "[";
}

static void print_end(const char *text = nullptr)
{
    std::cout << "]";
    if (text)
        std::cout << std::endl;
}

template <typename T>
static void print(T *v, int n, const char *text = nullptr)
{
    assert(v && n > 0);
    print_begin(text);
    do
    {
        std::cout << *v++;
        if (--n)
            std::cout << ", ";
    } while (n);
    print_end(text);
}

static void print_suit(int c)
{
#ifdef _MSC_VER
    auto suits{"\x03\x04\x05\x06"};
#else
    const char *suits[]{"\xE2\x99\xA0", "\xE2\x99\xA5", "\xE2\x99\xA6", "\xE2\x99\xA3"};
#endif
    std::cout << suits[c / 13];
};

static void print_rank(int c)
{
    std::cout << "A23456789TJQK"[c % 13];
};

static void print_deck(const std::vector<int> &v, const char *text = nullptr)
{
    auto show = [pos = 0](int c) mutable
    {
        if (pos++)
            std::cout << ' ';
        print_rank(c);
        print_suit(c);
    };
    print_begin(text);
    for_each(begin(v), end(v), show);
    print_end(text);
}

template <typename T>
static void print(const std::vector<T> &v, const char *text = nullptr)
{
    print_begin(text);
    auto n{0};
    for (const auto &e : v)
    {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template <typename T>
static void print(const std::vector<T> &v, int start, int end, const char *text = nullptr)
{
    print_begin(text);
    auto n{0};
    for (auto i = start; i <= end; ++i)
    {
        if (n++)
            std::cout << ",";
        std::cout << v[i];
    }
    print_end(text);
}

template <typename T>
static void print(const std::vector<std::vector<T>> &vv, const char *text = nullptr)
{
    print_begin(text);
    auto n{0};
    for (const auto &v : vv)
    {
        if (n++)
            std::cout << ",";
        print(v);
    }
    print_end(text);
}

template <typename T, size_t N>
static void print(const std::array<T, N> &a, const char *text = nullptr)
{
    print_begin(text);
    auto n{0};
    for (const auto &e : a)
    {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template <typename K, typename V>
static void print(const std::map<K, V> &m, const char *text = nullptr)
{
    print_begin(text);
    auto n{0};
    for (const auto &[key, value] : m)
    {
        if (n++)
            std::cout << ",";
        std::cout << "[" << key << "," << value << "]";
    }
    print_end(text);
}

template <typename K, typename V>
static void print(const std::unordered_map<K, V> &m, const char *text = nullptr)
{
    print_begin(text);
    auto n{0};
    for (const auto &[key, value] : m)
    {
        if (n++)
            std::cout << ",";
        std::cout << "[" << key << "," << value << "]";
    }
    print_end(text);
}

#endif // PRINT_H_
