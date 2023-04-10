// SPDX-License-Identifier: MIT
// g++ -std=c++23 -O3 -lstdc++ -lm list_node.cpp -o list_node
// g++ --version 9.3.0
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. list_node.cpp
// cl version 19.35.32215 for x64

#include "common.h"
#include "print.h"
#include "speedup.h"

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

void print(const ListNode* node, const char *text = nullptr) {
    if (text)
        std::cout << text << " = ";
    if (node) {
        std::cout << "[" << node->val;
        if (node->next) {
            std::cout << ", ";
            print(node->next);
        }
        std::cout << "]";
    }
    else
        std::cout << "nullptr";
    if (text)
        std::cout << std::endl;
}

class RandomSolution {
public:
    RandomSolution(ListNode* head) {
        m_head = head;
        auto n = 0;
        while (head) {
            head = head->next;
            n++;
        }
        m_size = n;
        std::srand(std::time(nullptr));
    }
    
    /** Returns a random node's value. */
    int getRandom() {
        auto v = std::rand() % m_size;
        if (v) {
            auto node = m_head->next;
            auto n = 1;
            while (node) {
                if (n == v)
                    return node->val;
                node = node->next;
                n++;
            }
        }
        return m_head->val;
    }

private:
    ListNode* m_head;
    int m_size;
};

class Solution {
public:
    void release(ListNode* head) {
        if (head) {
            release(head->next);
            delete head;
        }
    }

    ListNode* zerostrip(ListNode* head) {
        while (head && head->val == 0 && head->next) {
            auto next = head->next;
            delete head;
            head = next;
        }
        return head;
    }

    std::tuple<uint64_t, uint64_t> list2num(ListNode* node, uint64_t val = 0, int digit = 0, uint64_t mul = 1) {
        if (node && ++digit < 31) {
            auto [v, m] = list2num(node->next, val, digit, mul);
            val = v + (node->val * m);
            mul = m * 10;
        }
        return { val, mul };
    }

    uint64_t num2list(ListNode* head, uint64_t val, int digit = 0) {
        if (head && val && ++digit < 31) {
            auto node = new ListNode();
            if (node) {
                node->next = head->next;
                head->next = node;
                node->val = val - num2list(head, val / 10, digit);
            }
        }
        return val * 10;
    }

    ListNode* num2list(uint64_t val) {
        auto list = new ListNode();
        num2list(list, val);
        return zerostrip(list);
    }

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        auto [v1, m1] = list2num(l1);
        auto [v2, m2] = list2num(l2);
        return num2list(v1 + v2);
    }

    ListNode* sortList(ListNode* head) {
        auto list = head;
        std::map<int, int> map;
        while (list) {
            map[list->val]++;
            list = list->next;
        }
        auto fill = head;
        for (auto &e : map) {
            auto val = e.first;
            auto count = e.second;
            while (count--) {
                fill->val = val;
                fill = fill->next;
            }
        }
        return head;
    }

    ListNode* rotateRight(ListNode* head, int k) {
        if (!head)
            return nullptr;

        if (!head->next)
            return head;

        if (k < 1)
            return head;

        auto count = 1;
        auto tail = head;
        while (tail->next) { tail = tail->next; count++; };

        auto p0 = head;
        auto p1 = p0->next;
        auto skip = count - (k % count);
        if (skip == count)
            return head;

        while (--skip && p1) { p0 = p1; p1 = p1->next; }

        tail->next = head;
        p0->next = nullptr;
        head = p1;

        return head;
    }

    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        if (lists.empty())
            return nullptr;

        std::map<int, int> map;
        for (auto list : lists) {
            while (list) {
                map[list->val]++;
                list = list->next;
            }
        }

        ListNode* head = nullptr;
        ListNode* clone = nullptr;
        for (auto &e : map) {
            auto val = e.first;
            auto count = e.second;
            while (count--) {
                auto prev = clone;
                clone = new ListNode;
                if (!clone)
                    break;
                clone->val = val;
                clone->next = nullptr;
                if (prev)
                    prev->next = clone;
                if (!head)
                    head = clone;
            }
        }

        return head;
    }

    bool hasCycle(ListNode *head) {
        std::unordered_map<ListNode*, int> map;
        while (head) {
            if (map.find(head) != map.end())
                return true;
            map[head] = 1;
            head = head->next;
        }
        return false;        
    }

    bool hasCycleLeetCodeFastest(ListNode *head) {
        if (head == nullptr)
   	    	return false;
        ListNode *slow = head, *fast = head;
        while (slow && fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
                return true;
    	}
    	return false;
    }
};

static auto counter = 1;

void rotateRight(ListNode* node, int k) {
    std::cout << "Example " << counter++ << " rotateRight" << std::endl;
    Solution solution;
    print(node, "node");
    std::cout << "k = " << k << std::endl;
    auto result = solution.rotateRight(node, k);
    print(result, "result");
}

void rotateRight(uint64_t val, int k) {
    Solution solution;
    rotateRight(solution.num2list(val), k);
}

void f1(void) { rotateRight(12345, 2); }
void f2(void) { rotateRight(new ListNode(0, new ListNode(1, new ListNode(2))), 4); }
void f3(void) { rotateRight(12, 1); }
void f4(void) { rotateRight(12, 2); }
void f5(void) { rotateRight(12345, 3); }

void sortList(ListNode* head) {
    std::cout << "Example " << counter++ << " sortList" << std::endl;
    Solution solution;
    std::cout << "head = ";
    print(head);
    std::cout << std::endl;
    auto result = solution.sortList(head);
    std::cout << "result = ";
    print(result);
    std::cout << std::endl;
}

void sortList(uint64_t val) {
    Solution solution;
    sortList(solution.num2list(val));
}

void f6(void) { sortList(46315); }
void f7(void) { sortList(352); }
void f8(void) { sortList(72); }
void f9(void) { sortList(12); }
void f10(void) { sortList(new ListNode(4, new ListNode(3, new ListNode(1, new ListNode(12, new ListNode(5)))))); }
void f11(void) { sortList(new ListNode(4, new ListNode(19, new ListNode(14, new ListNode(5, new ListNode(-3, new ListNode(1, new ListNode(8, new ListNode(5, new ListNode(11, new ListNode(15))))))))))); }

void addTwoNumbers(uint64_t v1, uint64_t v2) {
    std::cout << "Example " << counter++ << " addTwoNumbers" << std::endl;
    Solution solution;
    auto l1 = solution.zerostrip(solution.num2list(v1));
    std::cout << "l1 = ";
    print(l1);
    std::cout << std::endl;
    auto l2 = solution.zerostrip(solution.num2list(v2));
    std::cout << "l2 = ";
    print(l2);
    std::cout << std::endl;
    auto result = solution.addTwoNumbers(l1, l2);
    std::cout << "l3 = ";
    print(result);
    std::cout << std::endl;
}

void f12(void) { addTwoNumbers(7243, 563); }
void f13(void) { addTwoNumbers(0, 0); }
void f14(void) { addTwoNumbers(9, 1999999999); }
void f15(void) { addTwoNumbers(3999999999,7); }

void getRandom(uint64_t v1) {
    std::cout << "Example " << counter++ << " getRandom" << std::endl;
    Solution factory;
    auto head = factory.zerostrip(factory.num2list(v1));
    print(head, "head");
    RandomSolution* solution = new RandomSolution(head);
    auto result = solution->getRandom();
    std::cout << "result = " << result << std::endl;
}

void f16(void) { getRandom(12345); }

void mergeKLists(const std::vector<int>& nums) {
    std::cout << "Example " << counter++ << " getRandom" << std::endl;
    Solution solution;
    std::vector<ListNode*> lists;
    for (auto &num : nums)
        lists.push_back(solution.num2list(num));
    for (auto &list : lists)
        print(list, "list");
    auto result = solution.mergeKLists(lists);
    print(result, "result");
    solution.release(result);
}

void f17(void) { mergeKLists({ 145, 134, 26 }); }

void hasCylce(ListNode* head) {
    std::cout << "Example " << counter++ << " sortList" << std::endl;
    Solution solution;
    auto result = solution.hasCycle(head);
    if (result == false)
        print(head, "head");
    std::cout << "result = " << result << std::endl;
}

void hasCycle(uint64_t val) {
    Solution solution;
    hasCylce(solution.num2list(val));
}

void f18(void) { hasCycle(12345); }

std::vector<void (*)(void)> examples {
    f18
};

int main() {
    for (auto &f : examples)
        f();
}
