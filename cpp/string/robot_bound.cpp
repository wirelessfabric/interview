// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -lstdc++ -lm robotBound.cpp -o robotBound
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Solution {
public:
    bool isRobotBounded(std::string instructions) {
        auto size = instructions.size();
        if (size == 1)
            return instructions[0] == 'G' ? false : true;

        typedef struct { int x; int y; int h; } move;
        typedef std::map<char, move> heading;
        const move moveNorth{0,1,0};
        const move moveWest{-1,0,1};
        const move moveSouth{0,-1,2};
        const move moveEast{1,0,3};
        const move turnNorth{0,0,0};
        const move turnWest{0,0,1};
        const move turnSouth{0,0,2};
        const move turnEast{0,0,3};
        const heading north{{'G',moveNorth},{'L',turnWest},{'R',turnEast}};
        const heading west{{'G',moveWest},{'L',turnSouth},{'R',turnNorth}};
        const heading south{{'G',moveSouth},{'L',turnEast},{'R',turnWest}};
        const heading east{{'G',moveEast},{'L',turnNorth},{'R',turnSouth}};
        const std::vector<heading> headings{north, west, south, east};

        move state{0,0,0};
        for (auto &c : instructions) {
            heading heading = headings[state.h];
            move move = heading[c];
            state.x += move.x;
            state.y += move.y;
            state.h = move.h;
        }

        return state.h || (!state.x && !state.y);
    }
};

static void example(const std::string &instructions) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    Solution solution;
    std::cout << "instructions = " << instructions << std::endl;
    auto result = solution.isRobotBounded(instructions);
    std::cout << "result " << result << std::endl;
    if (result)
        std::cout << "True" << std::endl;
    else
        std::cout << "False" << std::endl;
}

void example1(void) { example("GGLLGG"); }
void example2(void) { example("GG"); }
void example3(void) { example("GL"); }
void example4(void) { example("GLRLLGLL"); }

int main(int argc, char **argv) {
    if (argc == 2) {
        example(std::string(argv[1]));
    } else {
        example1();
        example2();
        example3();
        example4();
    }
}
