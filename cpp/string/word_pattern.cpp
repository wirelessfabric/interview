// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm word_pattern.cpp -o word_pattern
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

class Solution {
public:
    bool word_pattern(string pattern, string str) {
        unordered_map<char,string> keymap;
        unordered_map<string,char> wordmap;
        auto *k = pattern.c_str();
        auto *s = str.c_str();
        auto *w = s;
        auto *p = s;
        string word;
        char c;
        if (k && p) {
            do {
                word.clear();
                c = *p;
                switch (c) {
                case ' ':
                    p++;
                    word = str.substr(w-s, p-w-1);
                    w = p;
                    break;
                case '\0':
                    word = str.substr(w-s, p-w);
                    w = p;
                    break;
                default:
                    break;
                }
                p++;

                if (!word.empty()) {
                    auto key = *k++;
                    if (!key)
                        return false;

                    if (wordmap.find(word) == wordmap.end())
                        wordmap[word] = key;
                    else if (wordmap[word] != key)
                        return false;

                    if (keymap.find(key) == keymap.end())
                        keymap[key] = word;
                    else if (keymap[key] != word)
                        return false;
                }
            } while (c);
            return *k ? false : true;
        }
        return false;
    }
};

int main(int argc, char **argv) {
    if (argc > 2) {
        string pattern = std::string(argv[1]);

        string str;
        for (auto i=2; i < argc; i++) {
            if (i > 2)
                str += " ";
            str += std::string(argv[i]);
        }
        std::cout << "pattern = " << pattern << ", str = " << str << std::endl;

        Solution S;
        if (S.word_pattern(pattern, str))
            std::cout << "true" << std::endl;
        else
            std::cout << "false" << std::endl;
    }
}
