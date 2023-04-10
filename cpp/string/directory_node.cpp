// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -lstdc++ -lm directoryNode.cpp -o directoryNode
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <tuple>
#include <cstdint>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <random>

static const auto speedup = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

void print_begin(const char *text = nullptr) {
    if (text)
        std::cout << text << " = ";
    std::cout << "[";
}

void print_end(const char *text = nullptr) {
    std::cout << "]";
    if (text)
        std::cout << std::endl;
}

template<typename K, typename V>
void print(const std::unordered_map<K,V> &m, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &it : m) {
        if (n++)
            std::cout << ",";
        std::cout << "[" << it.first << "," << it.second << "]";
    }
    print_end(text);
}

struct DirectoryNode {
    std::unordered_map<std::string, DirectoryNode*> m_map;
    std::string m_name;
    DirectoryNode *m_parent;
    DirectoryNode(std::string name, DirectoryNode* parent) : m_name{name}, m_parent{parent} {
        if (parent)
            parent->m_map[name] = this;
    }
};

void print(const DirectoryNode* node, const char *text = nullptr) {
    print_begin(text);
    std::cout << " name = " << node->m_name;
    if (node->m_parent)
        std::cout << ", parent = " << node->m_parent->m_name;
    std::cout << ", ";
    print(node->m_map, "map");
    print_end(text);
}

static inline bool symbol(const char c) { return c == '.' || c == '_'; }
static inline bool number(const char c) { return c >= '0' && c <= '9'; }
static inline bool lowercase(const char c) { return c >= 'a' && c <= 'z'; }
static inline bool uppercase(const char c) { return c >= 'A' && c <= 'Z'; }
static inline bool alphabet(const char c) { return lowercase(c) || uppercase(c); }
static inline bool valid(const char c) { return alphabet(c) || number(c) || symbol(c); }

class Solution {
public:
    std::string simplifyPath(std::string path) {
        static const std::string nullstring{""};
        static const std::string rootstring{"/"};

        auto len = (int)path.size();
        if (!len)
            return nullstring;

        auto p = path.c_str();
        auto c = *p++;
        if (c != '/')
            return nullstring;

        if (len-- == 1)
            return rootstring;

        auto root = new DirectoryNode(rootstring, nullptr);
        auto node = root;
        while (node && len) {
            print(node, "node");
            while (*p == '/' && len) { p++; len--; }

            auto s = p;
            printf("s = [%s]\n", s);
            while ((c = *p) != '\0' && c != '/' && len) {
                if (!valid(c))
                    return nullstring;
                p++; len--;
            }

            if (s == p)
                break;

            std::string name(s, p-s);
            if (name == ".")
                continue;

            if (name == "..") {
                if (node->m_parent)
                    node = node->m_parent;
                continue;
            }

            auto parent = node->m_parent ? node->m_parent : root;
            auto it = parent->m_map.find(name);
            if (it == parent->m_map.end())
                node = new DirectoryNode(name, node);
            else
                node = it->second;
        }

        std::string canonical;
        while (node && node != root) {
            canonical = "/" + node->m_name + canonical;
            node = node->m_parent;
        }
        return canonical.empty() ? rootstring : canonical;
    }
};

static auto counter = 1;

void simplifyPath(std::string path) {
    std::cout << "simplifyPath" << counter++ << std::endl;
    std::cout << "path = " << path << std::endl;
    Solution solution;
    auto result = solution.simplifyPath(path);
    std::cout << "result = " << result << std::endl;
}

void f1(void) { simplifyPath("/"); }
void f2(void) { simplifyPath("//"); }
void f3(void) { simplifyPath("//a"); }
void f4(void) { simplifyPath("/a"); }
void f5(void) { simplifyPath("/a/b"); }
void f6(void) { simplifyPath("/a//b"); }
void f7(void) { simplifyPath("/a//b!/c"); }
void f8(void) { simplifyPath("/a/b/."); }
void f9(void) { simplifyPath("/a/b/./.."); }
void f10(void) { simplifyPath("/home/"); }
void f11(void) { simplifyPath("/home/foo/.ssh/../.ssh2/authorized_keys/"); }
void f12(void) { simplifyPath("/.///../JY"); }

std::vector<void (*)(void)> examples {
    /*f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f10, */ f12
};

int main() {
    for (auto &f : examples)
        f();
}
