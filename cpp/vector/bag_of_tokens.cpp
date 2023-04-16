// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -I.. -lstdc++ -lm bag_of_tokens.cpp -o bag_of_tokens
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

class Solution {
public:
    int playFaceUp(std::vector<int> &tokens, int P) {
        for (auto& t : tokens) {
            if (P <= 0)
                break;
            if (t <= 0)
                continue;
            if (P < t)
                continue;
            P -= t;
            t = -t;
        }
        return P;
    }

    int playFaceDown(std::vector<int> &tokens, int P) {
        std::cout << "playFaceDown input P" << P << std::endl;
        auto it = tokens.rbegin();
        for (; it != tokens.rend(); ++it) {
            auto t = *it;
            std::cout << "t=" << t << std::endl;
            if (t > 0) {
                P += t;
                *it = -t;
                break;
            }
        }
        std::cout << "playFaceDown return P" << P << std::endl;
        return P;
    }

    int bagOfTokensScore(std::vector<int> &tokens, int P) {
        std::sort(tokens.begin(), tokens.end());
        auto score = 0;

        auto P0 = P;
        auto P1 = playFaceUp(tokens, P);
        std::cout << "P0=" << P0 << " P1=" << P1 << std::endl;
        print(tokens, "tokens after playFaceUp");
        if (P0 == P1)
            return score;

        score++;
        auto P2 = playFaceDown(tokens, P1);
        print(tokens, "tokens after playFaceDown");
        std::cout << "P1=" << P1 << " P2=" << P1 << std::endl;
        return score;
    }
};

static void example(const std::vector<int> &arg, int P, int expect) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    auto tokens(arg);
    print(tokens, "tokens");
    std::cout << "P=" << P << std::endl;
    Solution solution;
    auto result = solution.bagOfTokensScore(tokens, P);
    std::cout << "result = " << result << std::endl;
    if (result != expect)
        std::cout << "expect = " << expect << std::endl;
}

static void f1(void) { example({100}, 50, 0); }
static void f2(void) { example({100,200}, 150, 1); }
static void f3(void) { example({100,200,300,400}, 200, 2); }
static void f4(void) { example({300,100,200,400}, 200, 2); }

static std::vector<void (*)(void)> examples {
    f1, f2, f3, f4
};

int main() {
    for (const auto& f : examples)
        f();
}
