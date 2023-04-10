// SPDX-License-Identifier: MIT
// g++ -std=c++17 -O3 -lstdc++ -lm treeNode.cpp -o treeNode
// g++ --version 9.3.0

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <iomanip>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

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

template<typename T> 
void print(const std::vector<T> &v, int start, int end, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (auto i=start; i <= end; ++i) {
        if (n++)
            std::cout << ",";
        std::cout << v[i];
    }
    print_end(text);
}

template<typename T, size_t N>
void print(const std::array<T, N> &a, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &e : a) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template<typename T>
void print(const std::vector<T> &v, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &e : v) {
        if (n++)
            std::cout << ",";
        std::cout << e;
    }
    print_end(text);
}

template<typename T> 
void print(const std::vector<std::vector<T>> &vv, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &v : vv) {
        if (n++)
            std::cout << ",";
        print(v);
    }
    print_end(text);
}

template<typename K, typename V>
void print(const std::map<K,V> &m, const char *text = nullptr) {
    print_begin(text);
    int n = 0;
    for (const auto &it : m) {
        if (n++)
            std::cout << ",";
        std::cout << "[" << it.first << "," << it.second << "]";
    }
    print_end(text);
}

void print(const TreeNode* node) {
    if (node) {
        std::cout << "[";
        if (node->left) {
            print(node->left);
            std::cout << ", ";
        }
        std::cout << node->val;
        if (node->right) {
            std::cout << ", ";
            print(node->right);
        }
        std::cout << "]";
    }
    else
        std::cout << "null";
}

void print(const TreeNode* node, const char *title) {
    std::cout << title << " = ";
    print(node);
    std::cout << std::endl;
}

static const auto speedup = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return nullptr;
}();

class Codec {
public:
    static constexpr auto nullValue = 16383;
    static constexpr auto nullString = "11111111111111";

    void encode(TreeNode* node, std::string& data) {
        if (node) {
            auto count = 14;
            auto value = node->val;
            while (value) {
                data += (value & 1) ? "1" : "0";
                value >>= 1;
                count--;
            }
            while (count--)
                data += "0";
            encode(node->left, data);
            encode(node->right, data);
        }
        else
            data += nullString;
    }

    int decodeValue(std::string& data, int& offset) {
        if (offset < data.size()) {
            auto value = 0;
            auto bit = 1;
            auto count = 14;
            while (count--) {
                if (data[offset++] == '1')
                    value |= bit;
                bit <<= 1;
            }
            return value;
        }
        return nullValue;
    }

    int decodeLeft(TreeNode* node, std::string& data, int offset) {
        if (node && offset < data.size()) {
            auto value = decodeValue(data, offset);
            if (value != nullValue) {
                node->left = new TreeNode(value);
                return decodeNode(node->left, data, offset);
            }
        }
        return offset;
    }

    int decodeRight(TreeNode* node, std::string& data, int offset) {
        if (node && offset < data.size()) {
            auto value = decodeValue(data, offset);
            if (value != nullValue) {
                node->right = new TreeNode(value);
                return decodeNode(node->right, data, offset);
            }
        }
        return offset;
    }

    int decodeNode(TreeNode* node, std::string& data, int offset) {
        return decodeRight(node, data, decodeLeft(node, data, offset));
    }

    std::string serialize(TreeNode* root) {
        std::string data;
        encode(root, data);
        return data;
    }

    TreeNode* deserialize(std::string data) {
        auto offset = 0;
        auto value = decodeValue(data, offset);
        if (value == nullValue)
            return nullptr;
        auto root = new TreeNode(value);
        offset = decodeNode(root, data, offset);
        return root;
    }
};

class Solution {
public:
    void maxAncestorDiff(const TreeNode* const node, int root, int& base, int& maxDiff) {
        if (!node)
            return;
        auto val = abs(node->val);
        if (val < root)
            base = val;
        const auto baseDiff = base > val ? base - val : val - base;
        if (baseDiff > maxDiff)
            maxDiff = baseDiff;
        if (node->left)
            maxAncestorDiff(node->left, root, base, maxDiff);
        if (node->right)
            maxAncestorDiff(node->right, root, base, maxDiff);
    }

    void maxAncestorDiff(TreeNode* node, int& maxDiff) {
        auto root = abs(node->val);
        auto base = root;
        if (node->left)
            maxAncestorDiff(node->left, root, base, maxDiff);
        if (node->right)
            maxAncestorDiff(node->right, root, base, maxDiff);
    }

    int maxAncestorDiff(TreeNode* root) {
        auto maxDiff = 0;
        maxAncestorDiff(root, maxDiff);
        return maxDiff;
    }

    int findSumOfTilts(const TreeNode* const node, int& sumOfTilts) {
        if (!node)
            return 0;
        const auto leftSum = node->left ? findSumOfTilts(node->left, sumOfTilts) : 0;
        const auto rightSum = node->right ? findSumOfTilts(node->right, sumOfTilts) : 0;
        if (leftSum > rightSum)
            sumOfTilts += leftSum - rightSum;
        else
            sumOfTilts += rightSum - leftSum;
        return leftSum + rightSum + node->val;
    }

    int findTilt(TreeNode* root) {
        auto sumOfTilts = 0;
        findSumOfTilts(root, sumOfTilts);
        return sumOfTilts;
    }

    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root)
            return new TreeNode(val);
        else if (val < root->val)
            root->left = insertIntoBST(root->left, val);
        else
            root->right = insertIntoBST(root->right, val);
        return root;
    }

    TreeNode* trimBST(TreeNode* root, int low, int high) {
        auto left = root->left;
        if (left)
            left = trimBST(left, low, high);
        auto right = root->right;
        if (right)
            right = trimBST(right, low, high);
        if (root->val < low || root->val > high) {
            delete root;
            return left ? left : right;
        }
        root->left = left;
        root->right = right;
        return root;
    }

    int addAsBits(std::vector<int>& v) {
        auto val = 0;
        for (auto& n : v)
            val = (val << 1) | n;
        return val;
    }

    int sumAllBranchesDepthFirst(std::vector<int>& v, TreeNode* t, int sum) {
        if (t && t->val >= 0 && t->val <= 1) {
            v.push_back(t->val);
            if (!t->left && !t->right) {
                sum += addAsBits(v);
            } else {
                if (t->left)
                    sum = sumAllBranchesDepthFirst(v, t->left, sum);
                if (t->right)
                    sum = sumAllBranchesDepthFirst(v, t->right, sum);
            }
            v.pop_back();
        }
        return sum;
    }

    int sumRootToLeaf(TreeNode* root) {
        std::vector<int> v;
        return sumAllBranchesDepthFirst(v, root, 0);
    }

    void getAllElementsDepthFirst(std::vector<int>* v, TreeNode* t) {
        if (t) {
            v->push_back(t->val);
            if (t->left)
                getAllElementsDepthFirst(v, t->left);
            if (t->right)
                getAllElementsDepthFirst(v, t->right);
        }
    }

    std::vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        std::vector<int> v;
        getAllElementsDepthFirst(&v, root1);
        getAllElementsDepthFirst(&v, root2);
        sort(v.begin(), v.end());
        return v;
    }
};

void getAllElements(TreeNode* root1, TreeNode* root2) {
    static auto counter = 1;
    std::cout << "getAllElements" << counter++ << std::endl;
    Solution solution;
    std::vector<int> v = solution.getAllElements(root1, root2);
    std::cout << "result =" << std::endl;
    for (auto& i : v)
        std::cout << i << std::endl;
}

void getAllElements1(void) {
    auto root1 = new TreeNode(2, new TreeNode(1), new TreeNode(4));
    auto root2 = new TreeNode(1, new TreeNode(0), new TreeNode(3));
    getAllElements(root1, root2);
}

void getAllElements2(void) {
    auto root1 = new TreeNode(0, new TreeNode(-10), new TreeNode(10));
    auto root2 = new TreeNode(5, new TreeNode(1), new TreeNode(7, new TreeNode(0), new TreeNode(2)));
    getAllElements(root1, root2);
}

void getAllElements3(void) {
    auto root1 = (TreeNode*)nullptr;
    auto root2 = new TreeNode(5, new TreeNode(1), new TreeNode(7, new TreeNode(0), new TreeNode(2)));
    getAllElements(root1, root2);
}

void getAllElements4(void) {
    auto root1 = new TreeNode(0, new TreeNode(-10), new TreeNode(10));
    auto root2 = (TreeNode*)nullptr;
    getAllElements(root1, root2);
}

void sumRootToLeaf(TreeNode* root) {
    static auto counter = 1;
    std::cout << "sumRootToLeaf" << counter++ << std::endl;
    Solution solution;
    auto result = solution.sumRootToLeaf(root);
    std::cout << "result = " << result << std::endl;
}

void sumRootToLeaf1(void) {
    sumRootToLeaf(new TreeNode(1,
        new TreeNode(0, new TreeNode(0), new TreeNode(1)),
        new TreeNode(1, new TreeNode(0), new TreeNode(1))));
}

void insertIntoBST(TreeNode* root, int val) {
    static auto counter = 1;
    std::cout << "insertIntoBST" << counter++ << std::endl;
    print(root, "root");
    std::cout << "val = " << val << std::endl;
    Solution solution;
    auto result = solution.insertIntoBST(root, val);
    print(result, "result");
}

void insertIntoBST1(void) {
    insertIntoBST(new TreeNode(4,
        new TreeNode(2, new TreeNode(1), new TreeNode(3)),
        new TreeNode(7)),
        5);
}

void insertIntoBST2(void) {
    insertIntoBST(new TreeNode(40,
        new TreeNode(20, new TreeNode(10), new TreeNode(30)),
        new TreeNode(60, new TreeNode(50), new TreeNode(70))),
        25);
}

void insertIntoBST3(void) {
    insertIntoBST(new TreeNode(4,
        new TreeNode(2, new TreeNode(1), new TreeNode(3)),
        new TreeNode(7)),
        5);
}

void insertIntoBST4(void) {
    insertIntoBST(nullptr, 5);
}

void trimBST(TreeNode* root, int low, int high) {
    static auto counter = 1;
    std::cout << "trimBST" << counter++ << std::endl;
    print(root, "root");
    std::cout << "low = " << low << " high = " << high << std::endl;
    Solution solution;
    auto result = solution.trimBST(root, low, high);
    print(result, "result");
}

void trimBST1(void) { trimBST(new TreeNode(1, new TreeNode(0), new TreeNode(2)), 1, 2); }
void trimBST2(void) { trimBST(new TreeNode(3, new TreeNode(0, nullptr, new TreeNode(2, new TreeNode(1), nullptr)), new TreeNode(4)), 1, 3); }
void trimBST3(void) { trimBST(new TreeNode(1), 1, 2); }
void trimBST4(void) { trimBST(new TreeNode(1, nullptr, new TreeNode(2)), 1, 3); }
void trimBST5(void) { trimBST(new TreeNode(1, nullptr, new TreeNode(2)), 2, 4); }

void serdes(TreeNode* root) {
    static auto counter = 1;
    std::cout << "serdes" << counter++ << std::endl;
    print(root, "root");
    Solution solution;
    auto ser = new Codec();
    auto des = new Codec();
    std::string tree = ser->serialize(root);
    auto result = des->deserialize(tree);
    print(result, "result");
}

void serdes1(void) { serdes(new TreeNode(1, new TreeNode(2), new TreeNode(3))); }
void serdes2(void) { serdes(nullptr); }

void findTilt(TreeNode* root, int expect) {
    static auto counter = 1;
    std::cout << "findTilt" << counter++ << std::endl;
    print(root, "root");
    Solution solution;
    auto result = solution.findTilt(root);
    std::cout << "result = " << result << std::endl;
    if (result != expect)
        std::cout << "expect = " << expect << std::endl;
}

void findTilt1(void) {
    findTilt(new TreeNode(1, new TreeNode(2), new TreeNode(3)), 1);
}
void findTilt2(void) {
    findTilt(new TreeNode(4,
                new TreeNode(2, new TreeNode(3), new TreeNode(5)),
                new TreeNode(9, nullptr, new TreeNode(7))),
             15);
}
void findTilt3(void) {
    findTilt(new TreeNode(21,
                new TreeNode(7,
                    new TreeNode(1, new TreeNode(3), new TreeNode(3)),
                    new TreeNode(1)),
                new TreeNode(14, new TreeNode(2), new TreeNode(2))),
             9);
}
void findTilt4(void) {
    findTilt(new TreeNode(1, new TreeNode(2, new TreeNode(3, new TreeNode(4, new TreeNode(5), nullptr), nullptr), nullptr), nullptr), 40);
}

void maxAncestorDiff(TreeNode* root, int expect) {
    static auto counter = 1;
    std::cout << "maxAncestorDiff" << counter++ << std::endl;
    print(root, "root");
    Solution solution;
    auto result = solution.maxAncestorDiff(root);
    std::cout << "result = " << result << std::endl;
    if (result != expect)
        std::cout << "expect = " << expect << std::endl;
}

void maxAncestorDiff1(void) {
    maxAncestorDiff(new TreeNode(8,
                        new TreeNode(3, new TreeNode(1), new TreeNode(6, new TreeNode(4), new TreeNode(7))),
                        new TreeNode(10, nullptr, new TreeNode(14, new TreeNode(13), nullptr))), 7);
}
void maxAncestorDiff2(void) {
    maxAncestorDiff(new TreeNode(1, nullptr, new TreeNode(2, nullptr, new TreeNode(0, new TreeNode(3), nullptr))), 3);
}
void maxAncestorDiff3(void) {
    maxAncestorDiff(new TreeNode(1, new TreeNode(2), new TreeNode(3)), 2);
}
void maxAncestorDiff4(void) {
    maxAncestorDiff(new TreeNode(2, new TreeNode(5), new TreeNode(0, new TreeNode(4, nullptr, new TreeNode(6, new TreeNode(1, new TreeNode(3), nullptr), nullptr)), nullptr)), 6);
}
void maxAncestorDiff5(void) {
    maxAncestorDiff(new TreeNode(8,
                        new TreeNode(3, new TreeNode(1), new TreeNode(6, new TreeNode(4), new TreeNode(7))),
                        new TreeNode(10, nullptr, new TreeNode(14, new TreeNode(13), nullptr))), 7);
}
void maxAncestorDiff6(void) { // FAILS
    maxAncestorDiff(new TreeNode(2,
                        new TreeNode(4, new TreeNode(1, nullptr, new TreeNode(6)), nullptr),
                        new TreeNode(3, new TreeNode(0), new TreeNode(5, nullptr, new TreeNode(7)))), 5);
}

std::vector<void (*)(void)> examples{
    trimBST1, trimBST2, trimBST3, trimBST4, trimBST5
};

int main() {
    for (auto& f : examples)
        f();
}
