// SPDX-License-Identifier: MIT
// g++ -std=c++2a -O3 -I.. -lstdc++ -lm image_overlap.cpp -o image_overlap
// g++ --version 11.3.0 on soho ubuntu 22.04

#include "common.h"
#include "print.h"
#include "speedup.h"

using namespace std;

class Solution {
public:
    int largestOverlap(vector<vector<int>> &A, vector<vector<int>> &B) {
        auto n = A.size();
        print(A, "A");
        print(B, "B");
        auto C = std::move(allocate(n));
        auto max = 0;
        for (auto r=0; r <= n; ++r) {
            for (auto c=0; c <= n; ++c) {
                max = translate(B, C, A,  r,  c, max, n);
                max = translate(B, C, A,  r, -c, max, n);
                max = translate(B, C, A, -r,  c, max, n);
                max = translate(B, C, A, -r, -c, max, n);
                max = translate(A, C, B,  r,  c, max, n);
                max = translate(A, C, B,  r, -c, max, n);
                max = translate(A, C, B, -r,  c, max, n);
                max = translate(A, C, B, -r, -c, max, n);
            }
        }
        return max;
    }

private:
    int overlap(vector<vector<int>> &a, vector<vector<int>> &b, int max, int n) {
        auto count = 0;
        for (auto r=0; r < n; ++r)
            for (auto c=0; c < n; ++c)
                if (a[r][c] == 1 && b[r][c] == 1)
                    count++;
        return count > max ? count : max;
    }

    void copy(vector<vector<int>> &dst, vector<vector<int>> &src, int n) {
        for (auto r=0; r < n; ++r)
            for (auto c=0; c < n; ++c)
                dst[r][c] = src[r][c];
    }

    void clear(vector<vector<int>> &dst, int n) {
        for (auto r=0; r < n; ++r)
            for (auto c=0; c < n; ++c)
                dst[r][c] = 0;
    }

    vector<vector<int>> allocate(size_t n) {
        vector<vector<int>> img;
        img.reserve(n);
        for (auto i=0; i < n; ++i) {
            vector<int> row;
            row.reserve(n);
            img.emplace_back(std::move(row));
        }
        clear(img, n);
        return img;
    }

    void right(vector<vector<int>> &dst, vector<vector<int>> &src, int h, int n) {
        for (auto r=0; r < n; ++r)
            for (auto c=0; c < n; ++c)
                dst[r][c] = c < h ? 0 : src[r][c-h];
    }

    void left(vector<vector<int>> &dst, vector<vector<int>> &src, int h, int n) {
        for (auto r=0; r < n; ++r)
            for (auto c=0; c < n; ++c)
                dst[r][c] = c < n-h ? src[r][c+1] : 0;
    }

    void up(vector<vector<int>> &dst, vector<vector<int>> &src, int v, int n) {
        for (auto r=0; r < n; ++r)
            for (auto c=0; c < n; ++c)
                dst[r][c] = r < n-v ? src[r+1][c] : 0;
    }

    void down(vector<vector<int>> &dst, vector<vector<int>> &src, int v, int n) {
        for (auto r=n-1; r >= 0; --r)
            for (auto c=0; c < n; ++c)
                dst[r][c] = r < v ? 0 : src[r-1][c];
    }

    int translate(vector<vector<int>> &z, vector<vector<int>> &y, vector<vector<int>> &x, int h, int v, int max, int n) {
        if (h > 0)
            right(y, x, h, n);
        else if (h < 0)
            left(y, x, -h, n);
        else
            copy(y, x, n);

        if (v > 0)
            up(y, y, v, n);
        else if (v < 0)
            down(y, y, -v, n);

        return overlap(y, z, max, n);
    }
};

#if 0 // https://leetcode.com/problems/image-overlap/solution/
class Solution {
    protected int shiftAndCount(int xShift, int yShift, int[][] M, int[][] R) {
        int count = 0;
        int rRow = 0;
        // count the cells of ones in the overlapping zone.
        for (int mRow = yShift; mRow < M.length; ++mRow) {
            int rCol = 0;
            for (int mCol = xShift; mCol < M.length; ++mCol) {
                if (M[mRow][mCol] == 1 && M[mRow][mCol] == R[rRow][rCol])
                    count += 1;
                rCol += 1;
            }
            rRow += 1;
        }
        return count;
    }

    public int largestOverlap(int[][] A, int[][] B) {
        int maxOverlaps = 0;

        for (int yShift = 0; yShift < A.length; ++yShift)
            for (int xShift = 0; xShift < A.length; ++xShift) {
                // move one of the matrice up and left and vice versa.
                // (equivalent to move the other matrix down and right)
                maxOverlaps = Math.max(maxOverlaps, shiftAndCount(xShift, yShift, A, B));
                maxOverlaps = Math.max(maxOverlaps, shiftAndCount(xShift, yShift, B, A));
            }

        return maxOverlaps;
    }
}
#endif

void example(vector<vector<int>>& A, vector<vector<int>>& B) {
    Solution solution;
    auto overlap = solution.largestOverlap(A, B);
    std::cout << overlap << std::endl;
}

void example1(void) {
    vector<vector<int>> A{{1,1,1},{0,1,0},{0,1,0}};
    vector<vector<int>> B{{0,0,0},{0,1,1},{0,0,1}};
    example(A, B);
}

void example2(void) {
    vector<vector<int>> A{{1,0},{1,0}};
    vector<vector<int>> B{{0,1},{1,0}};
    example(A, B);
}


void example47(void) {
    vector<vector<int>> A{{1,0,1,0,1,0,1,1,0,0,0,0,0,0,0},
                          {1,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
                          {1,0,1,0,0,0,0,0,0,0,1,0,0,1,0},
                          {1,0,0,0,0,0,0,0,1,0,0,1,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                          {1,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
                          {0,1,1,0,1,1,0,0,1,0,1,0,0,0,0},
                          {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                          {1,0,0,0,0,1,0,0,0,0,0,1,1,0,1},
                          {0,0,0,0,1,0,0,0,0,1,0,1,0,1,0},
                          {1,1,1,0,0,0,0,0,1,0,0,0,0,0,0},
                          {1,0,0,0,1,0,1,0,1,0,1,0,0,0,1},
                          {1,1,1,0,1,0,1,0,0,0,0,0,1,0,0},
                          {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
                          {0,1,1,1,0,0,0,1,0,0,0,0,0,0,0}};

    vector<vector<int>> B{{0,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
                          {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
                          {0,0,0,0,0,0,1,0,1,0,0,0,0,1,0},
                          {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
                          {1,1,0,0,1,0,0,0,0,0,0,0,1,0,1},
                          {1,0,1,0,0,0,0,0,0,0,0,1,0,1,1},
                          {0,0,0,0,0,1,0,0,1,0,0,1,1,0,1},
                          {0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
                          {0,1,1,0,0,0,1,0,0,0,0,1,0,0,0},
                          {0,1,0,1,0,0,1,0,0,0,1,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                          {0,0,1,0,1,0,0,1,0,0,1,0,0,1,0},
                          {1,0,1,0,0,1,0,0,0,0,0,0,0,1,0}};
    example(A, B);
}

int main() {
    example1();
    example2();
    example47();
    return 0;
}
