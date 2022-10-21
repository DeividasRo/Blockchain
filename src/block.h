#pragma once
#include <string>
#include <vector>

using std::string;

template <typename T>
class Block
{
private:
    string previous_block_hash;
    string merkel_root_hash;
    string version;
    unsigned int timestamp;
    int nonce;
    float difficulty_target;
    std::vector<T> transactions;

public:
    Block() : previous_block_hash(""), merkel_root_hash(""), version(""), timestamp(0), nonce(0), difficulty_target(0)
    {
    }

    Block(string previous_block_hash, string merkel_root_hash, string version, unsigned int timestamp, int nonce, float difficulty_target)
    {
        this->previous_block_hash = previous_block_hash;
        this->merkel_root_hash = merkel_root_hash;
        this->version = version;
        this->timestamp = timestamp;
        this->nonce = nonce;
        this->difficulty_target = difficulty_target;
    }

    // getters
    inline string GetPreviousBlockHash() const { return previous_block_hash; }
    inline string GetMerkelRootHash() const { return merkel_root_hash; }
    inline string GetVersion() const { return version; }
    inline unsigned int GetTimestamp() const { return timestamp; }
    inline int GetNonce() const { return nonce; }
    inline float GetDifficultyTarget() const { return difficulty_target; }
    inline std::vector<T> GetTransactions() const { return transaction; }
};