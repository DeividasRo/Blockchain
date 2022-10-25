#pragma once
#include "user.h"
#include "transaction.h"
#include "block.h"
#include "myhash.h"
#include "generator.h"
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iostream>

using std::string;

class Blockchain
{
private:
    string name;
    std::vector<User> users;
    std::vector<Transaction> transactions;
    std::vector<Block> blocks;

public:
    Blockchain() : name("unnamed") {}

    Blockchain(string name)
    {
        this->name = name;
    }

    void CreateBlock(int difficulty_target, int version = 1);

    inline void GenerateBlockchainUsers(int amount) { GenerateUsers(users, amount); }
    inline void GenerateUnverifiedTransactions(int amount) { GenerateTransactions(transactions, users, amount); }

    inline int UserCount() { return users.size(); }
    inline int TransactionCount() { return transactions.size(); }
    inline int BlockCount() { return blocks.size(); }

    // getters
    inline std::vector<User> GetUsers() const { return users; }
    inline std::vector<Transaction> GetTransactions() const { return transactions; }
    inline std::vector<Block> GetBlocks() const { return blocks; }
};

string IntToHexString(unsigned int value);
