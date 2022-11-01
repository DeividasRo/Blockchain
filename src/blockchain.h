#pragma once
#include "user.h"
#include "transaction.h"
#include "block.h"
#include "myhash.h"
#include "helpers.h"
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>

using std::string;

class Blockchain
{
private:
    int difficulty_target;
    int version;
    std::vector<User> users;
    std::vector<Transaction> transaction_pool;
    std::vector<Block> blocks;

public:
    Blockchain() : difficulty_target(4) {}

    void CreateBlocks();

    // Mining
    void MineBlock(string miner_name, int initial_block_count);

    // Generators
    void GenerateUsers(int amount);
    void GenerateTransactionPool(int amount);

    // Saving data to .txt files
    void SaveUsersData();
    void SaveTransactionPoolData();
    void SaveBlocksData();

    // Loading data from .txt files
    void LoadUsersData();
    void LoadTransactionPoolData();
    void LoadBlocksData();

    // Clear all contents of blockchain
    void ClearBlockchain()
    {
        users.clear();
        transaction_pool.clear();
        blocks.clear();
        users.shrink_to_fit();
        transaction_pool.shrink_to_fit();
        blocks.shrink_to_fit();
    }

    // Return the amount of elements in a container
    inline int UserCount() { return users.size(); }
    inline int TransactionPoolCount() { return transaction_pool.size(); }
    inline int BlockCount() { return blocks.size(); }

    // setters
    inline void SetDifficultyTarget(int difficulty_target) { this->difficulty_target = difficulty_target; }
    inline void SetVersion(int version) { this->version = version; }

    // getters
    inline int GetDifficultyTarget() const { return difficulty_target; }
    inline int GetVersion() const { return version; }
    inline std::vector<User> GetUsers() const { return users; }
    inline std::vector<Transaction> GetTransactionPool() const { return transaction_pool; }
    inline Transaction GetTransactionFromPool(int idx) const { return transaction_pool[idx]; }
    inline std::vector<Block> GetBlocks() const { return blocks; }
    inline Block GetBlock(int idx) const { return blocks[idx]; }
};

string MerkleRootHash(std::vector<string> merkle);
