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
#include <fstream>

using std::string;

class Blockchain
{
private:
    int difficulty_target;
    std::vector<User> users;
    std::vector<Transaction> transaction_pool;
    std::vector<Block> blocks;

public:
    Blockchain() : difficulty_target(4) {}

    void CreateBlock(int difficulty_target, int version = 1);
    void SaveUsersData();
    void SaveTransactionPoolData();
    void SaveBlocksData();
    void LoadUsersData();
    void LoadTransactionPoolData();
    void LoadBlocksData();

    void ClearBlockchain()
    {
        users.clear();
        transaction_pool.clear();
        blocks.clear();
        users.shrink_to_fit();
        transaction_pool.shrink_to_fit();
        blocks.shrink_to_fit();
    }

    inline void GenerateBlockchainUsers(int amount) { GenerateUsers(users, amount); }
    inline void GenerateTransactionPool(int amount) { GenerateTransactions(transaction_pool, users, amount); }

    inline int UserCount() { return users.size(); }
    inline int TransactionPoolCount() { return transaction_pool.size(); }
    inline int BlockCount() { return blocks.size(); }

    // setters
    inline void SetDifficultyTarget(int difficulty_target) { this->difficulty_target = difficulty_target; }

    // getters
    inline int GetDifficultyTarget() const { return difficulty_target; }
    inline std::vector<User> GetUsers() const { return users; }
    inline std::vector<Transaction> GetTransactionPool() const { return transaction_pool; }
    inline Transaction GetTransactionFromPool(int idx) const { return transaction_pool[idx]; }
    inline std::vector<Block> GetBlocks() const { return blocks; }
    inline Block GetBlock(int idx) const { return blocks[idx]; }
};

string IntToHexString(unsigned int value);
