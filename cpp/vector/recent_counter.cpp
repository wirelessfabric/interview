// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm recentCounter.cpp -o recentCounter
// g++ --version 9.3.0

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <climits>

class RecentCounterLeetcodeFastest {
public:
    RecentCounterLeetcodeFastest() {
        m_size = 0;
    }
    
    int ping(int t) {
        while (!m_queue.empty() && m_queue.front() < t - 3000) {
            m_size--;
            m_queue.pop();
        }
        m_queue.push(t);
        m_size++;
        return m_size;
    }
private:
    std::queue<int> m_queue;
    int m_size;
};

class RecentCounter {
public:
    RecentCounter() {
        result.clear();
    }

    int ping(int t) {
        result.emplace_back(t);

        auto base = t - 3000;
        auto i = result.size();
        while (i--)
            if (result[i] < base)
                break;

        return result.size() - i - 1;
    }

private:
    std::vector<int> result;
};

class Solution {
public:
    template<typename T> 
    void print(const std::vector<T> &v, int start, int end) {
        std::cout << "[";
        int n = 0;
        for (auto i=start; i <= end; ++i) {
            if (n++)
                std::cout << ",";
            std::cout << v[i];
        }
        std::cout << "]";
    }

    template<typename T> 
    void print(const std::vector<T> &v) {
        std::cout << "[";
        int n = 0;
        for (const auto &e : v) {
            if (n++)
                std::cout << ",";
            std::cout << e;
        }
        std::cout << "]";
    }

    template<typename T> 
    void print(const std::vector<std::vector<T>> &vv) {
        std::cout << "[";
        int n = 0;
        for (const auto &v : vv) {
            if (n++)
                std::cout << ",";
            print(v);
        }
        std::cout << "]";
    }
};

static void example(std::vector<std::string> &op, std::vector<std::vector<int>> &data) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "op = ";
    solution.print(op);
    std::cout << std::endl;
    std::cout << "data = ";
    solution.print(data);
    std::cout << std::endl;

    std::vector<int> result;
    RecentCounter *obj = nullptr;
    for (auto i=0; i < op.size(); ++i) {
        if (op[i] == "RecentCounter") {
            result.clear();
            result.emplace_back(0);

            if (obj)
                delete obj;
            obj = new RecentCounter();
            if (obj)
                continue;
            break;
        }

        if (op[i] == "ping")
            for (auto &v : data[i])
                result.emplace_back(obj->ping(v));
    }

    std::cout << "result = ";
    solution.print(result);
    std::cout << std::endl;
}

void example1(void) {
    std::vector<std::string> op = {"RecentCounter", "ping", "ping", "ping", "ping"};
    std::vector<std::vector<int>> data{{}, {1}, {100}, {3001}, {3002}};
    example(op, data);
}

int main(int argc, char **argv) {
    example1();
}
