// SPDX-License-Identifier: MIT
// g++ -std=c++23 -O3 -lstdc++ -lm rollback_consistency.cpp -o rollback_consistency
// g++ --version 9.3.0
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc /std:c++20 /I.. rollback_consistency.cpp
// cl version 19.35.32215 for x64

#include "common.h"
#include "speedup.h"

struct account_balance {
    int account_id;
    int balance;
};

struct transfer {
    int from;
    int to;
    int amount;
};

using transaction = std::vector<transfer>;

struct transaction_record {
    int id;
    bool reverted;
    std::vector<transfer> transfers;
};

struct transfer_record {
    int id;
    int amount;
    int balance;
    int xid;
};

struct account_record {
    int id;
    int balance;
    std::vector<transfer_record> transfers;
};

class database {
public:
    inline auto assign_xid() {
        return ++transaction_id;
    }

    inline auto assign_tid() {
        return ++transfer_id;
    }

    inline void credit_account(int aid, int amount, int tid, int xid) {
        auto& account = accounts.at(aid);
        account.balance += amount;
        account.transfers.push_back({tid, amount, account.balance, xid});
    }

    inline void debit_account(int aid, int amount, int tid, int xid) {
        auto& account = accounts.at(aid);
        account.balance -= amount;
        account.transfers.push_back({tid, -amount, account.balance, xid});
    }

    inline void create_account(int aid) {
        if (accounts.find(aid) == accounts.end()) {
            auto& account = accounts[aid];
            account.id = aid;
            account.balance = 0;
            account.transfers.clear();
        }
    }

    inline int xid2index(int xid) const {
        return xid > 0 ? xid-1 : xid < 0 ? -xid-1 : 0;
    }

    void revert_transaction(transaction_record &x) {
        for (const auto &t : x.transfers) {
            auto tid = assign_tid();
            debit_account(t.to, t.amount, tid, -x.id);
            credit_account(t.from, t.amount, tid, -x.id);
        }
        x.reverted = true;
    }

    // push a transaction to the database
    void push_transaction( const transaction&t ) {
        transaction_record x;
        x.id = assign_xid();
        x.reverted = false;
        for (const auto &transfer : t) {
            auto tid = assign_tid();
            debit_account(transfer.from, transfer.amount, tid, x.id);
            credit_account(transfer.to, transfer.amount, tid, x.id);
            x.transfers.push_back(transfer);
        }
        transactions.push_back(x);
    }

    void settle_account(account_record &account) {
        auto t = account.transfers.crbegin();
        for (; t != account.transfers.crend(); ++t) {
            if (t->balance >= 0)
                continue;

            auto& x = transactions.at(xid2index(t->xid));
            if (x.reverted)
                continue;

            revert_transaction(x);
        }
    }

    // settle the database such that invariant is maintained and best state of the database is present
    void settle() {
        auto retries = transactions.size();
        do {
            auto settled = 0;
            for (const auto &it : accounts) {
                auto account = it.second;
                if (account.id > 0 && account.balance < 0) {
                    settle_account(account);
                    break;
                }
                settled++;
            }
            if (settled == accounts.size())
                break;
        } while (--retries);
    }

    // return a listing of all balances in any order
    std::vector<account_balance> get_balances() const {
        std::vector<account_balance> v;
        for (const auto &it : accounts) {
            auto account = it.second;
            if (account.id > 0)
                v.push_back({account.id, account.balance});
        }
        return v;
    }

    // Given the logical sequence of transactions constructed by the ordered sequence of calls
    // to push_transaction AND having called settle()
    // 
    // return the 0-based indices of the surviving transactions in that sequence which, when 
    // applied to the initial balances produce a state where the invariant is maintained
    //
    std::vector<size_t> get_applied_transactions() const {
        std::vector<size_t> v;
        for (const auto &x : transactions)
            if (!x.reverted)
                v.push_back(xid2index(x.id));
        return v;
    }

    void print_account_transfer(const transfer_record &t) const {
        std::cout << "[" << t.id << "," << t.amount << "," << t.balance << "," << t.xid << "]";
    }

    void print_account_transfers(const std::vector<transfer_record> &transfers) const {
        auto n = 0;
        std::cout << "{";
        for (const auto &t : transfers) {
            if (n++)
                std::cout << ",";
            print_account_transfer(t);
        }
        std::cout << "}";
    }

    void print_account_record(const account_record &account) const {
        std::cout << "[" << account.id << "," << account.balance << ",";
        print_account_transfers(account.transfers);
        std::cout << "]";
    }

    void print_accounts(const std::map<int, account_record> &m) const {
        auto n = 0;
        std::cout << "[";
        for (const auto &it : m) {
            if (n++)
                std::cout << ",";
            auto account = it.second;
            print_account_record(account);
        }
        std::cout << "]";
    }

    void print_transaction_transfer(const transfer &t) const {
        std::cout << "[" << t.from << "," << t.to << "," << t.amount << "]";
    }

    void print_transaction_transfers(const std::vector<transfer> &v) const {
        auto n = 0;
        std::cout << "{";
        for (const auto &t : v) {
            if (n++)
                std::cout << ",";
            print_transaction_transfer(t);
        }
        std::cout << "}";
    }

    void print_transaction_records(const std::vector<transaction_record> &v) const {
        auto n = 0;
        std::cout << "[";
        for (const auto &x : v) {
            if (n++)
                std::cout << ",";
            std::cout << "[" << x.id;
            if (x.reverted)
                std::cout << " reverted";
            std::cout << ",";
            print_transaction_transfers(x.transfers);
            std::cout << "]";
        }
        std::cout << "]";
    }

    void print_account_balances(const std::vector<account_balance> &v) const {
        std::cout << "[";
        int n = 0;
        for (const auto &e : v) {
            if (n++)
                std::cout << ",";
            std::cout << "[" << e.account_id << "," << e.balance << "]";
        }
        std::cout << "]";
    }

private:
    std::map<int, account_record> accounts;
    std::vector<transaction_record> transactions;
    int transaction_id{0};
    int transfer_id{0};
};

auto create_database(const std::vector<account_balance> &initial_balances) {
    auto xid = 0;
    database db;
    db.create_account(0);
    for (auto &initial : initial_balances) {
        auto tid = db.assign_tid();
        db.debit_account(0, initial.balance, tid, xid);
        db.create_account(initial.account_id);
        db.credit_account(initial.account_id, initial.balance, tid, xid);
    }
    return db;
}

void example(const std::vector<account_balance> &initial_balances, const std::vector<transaction> &transactions) {
    static auto counter = 1;
    std::cout << "Example " << counter++ << std::endl;
    std::cout << std::endl;
    auto db = create_database(initial_balances);
    for (const auto &t: transactions)
        db.push_transaction(t);
    db.settle();
}

void f1(void) { example({{1,5},{2,10},{3,15}}, {{{1,2,3},{3,1,2}}}); }
void f2(void) { example({{1,5},{2,10},{3,15}}, {{{1,2,3},{3,1,2}}, {{2,1,11}}}); }
void f3(void) { example({{1,5},{2,10},{3,15}}, {{{2,1,11}}}); }
void f4(void) { example({{1,5},{2,10},{3,15}}, {{{2,1,11}},{{1,2,3},{3,1,2}}}); }
void f5(void) { example({{1,5},{2,10},{3,15}}, {{{2,1,11}},{{1,2,3},{3,1,2}}}); }
void f6(void) { example({{1,10},{2,5}}, {{{1,2,5}}}); }

std::vector<void (*)(void)> examples {
    f1, f2, f3, f4, f5, f6
};

int main() {
    for (const auto& f : examples)
        f();
}
