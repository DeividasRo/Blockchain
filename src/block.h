#pragma once
#include <string>
#include <vector>

using std::string;

class Block
{
private:
    string current_block_hash;
    string previous_block_hash;
    string merkel_root_hash;
    unsigned int timestamp;
    unsigned int nonce;
    int difficulty_target;
    int height;
    int version;
    std::vector<Transaction> transactions;

public:
    Block() : current_block_hash(""), previous_block_hash(""), merkel_root_hash(""), timestamp(0), nonce(0), difficulty_target(0), height(0), version(1) {}

    Block(string current_block_hash, string previous_block_hash, string merkel_root_hash, unsigned int timestamp, unsigned int nonce, int difficulty_target, int height, int version)
    {
        this->current_block_hash = current_block_hash;
        this->previous_block_hash = previous_block_hash;
        this->merkel_root_hash = merkel_root_hash;
        this->timestamp = timestamp;
        this->nonce = nonce;
        this->difficulty_target = difficulty_target;
        this->height = height;
        this->version = version;
    }

    // setters
    inline void SetTransactions(std::vector<Transaction> transactions) { this->transactions = transactions; }
    // getters
    inline string GetCurrentBlockHash() const { return current_block_hash; }
    inline string GetPreviousBlockHash() const { return previous_block_hash; }
    inline string GetMerkelRootHash() const { return merkel_root_hash; }
    inline unsigned int GetTimestamp() const { return timestamp; }
    inline int GetNonce() const { return nonce; }
    inline int GetDifficultyTarget() const { return difficulty_target; }
    inline int GetHeight() const { return height; }
    inline int GetVersion() const { return version; }
    inline std::vector<Transaction> GetTransactions() const { return transactions; }
};