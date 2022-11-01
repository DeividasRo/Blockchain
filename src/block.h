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
    string miner;
    unsigned int timestamp;
    unsigned long long nonce;
    int difficulty_target;
    int height;
    int version;
    int transaction_count;
    std::vector<Transaction> transactions;

public:
    Block() : current_block_hash(""), previous_block_hash(""), merkel_root_hash(""), miner(""), timestamp(0), nonce(0), difficulty_target(0), height(0), version(1), transaction_count(0) {}

    Block(string current_block_hash, string previous_block_hash, string merkel_root_hash, string miner, unsigned int timestamp, unsigned long long nonce, int difficulty_target, int height, int version, std::vector<Transaction> transactions)
    {
        this->current_block_hash = current_block_hash;
        this->previous_block_hash = previous_block_hash;
        this->merkel_root_hash = merkel_root_hash;
        this->miner = miner;
        this->timestamp = timestamp;
        this->nonce = nonce;
        this->difficulty_target = difficulty_target;
        this->height = height;
        this->version = version;
        this->transactions = transactions;
        this->transaction_count = transactions.size();
    }

    ~Block()
    {
        transactions.clear();
        transactions.shrink_to_fit();
    }

    friend std::istream &operator>>(std::istream &in, Block &b)
    {
        in >> b.current_block_hash >> b.previous_block_hash >> b.merkel_root_hash >> b.miner >> b.timestamp >> b.nonce >> b.difficulty_target >> b.height >> b.version >> b.transaction_count;
        for (int i = 0; i < b.transaction_count; i++)
        {
            Transaction t;
            string s1, s2, s3;
            int i1;
            in >> s1 >> s2 >> s3 >> i1;
            t.SetTransactionId(s1);
            t.SetSenderKey(s2);
            t.SetReceiverKey(s3);
            t.SetValue(i1);
            b.transactions.push_back(t);
        }
        return in;
    }

    inline int TransactionCount() const { return transaction_count; };

    // setters
    inline void SetCurrentBlockHash(string current_block_hash) { this->current_block_hash = current_block_hash; }
    inline void SetPreviousBlockHash(string previous_block_hash) { this->previous_block_hash = previous_block_hash; }
    inline void SetMerkelRootHash(string merkel_root_hash) { this->merkel_root_hash = merkel_root_hash; }
    inline void SetTransactions(std::vector<Transaction> transactions) { this->transactions = transactions; }

    // getters
    inline string GetCurrentBlockHash() const { return current_block_hash; }
    inline string GetPreviousBlockHash() const { return previous_block_hash; }
    inline string GetMerkelRootHash() const { return merkel_root_hash; }
    inline string GetMiner() const { return miner; }
    inline unsigned int GetTimestamp() const { return timestamp; }
    unsigned long long GetNonce() const { return nonce; }
    inline int GetDifficultyTarget() const { return difficulty_target; }
    inline int GetHeight() const { return height; }
    inline int GetVersion() const { return version; }
    inline std::vector<Transaction> GetTransactions() const { return transactions; }
    inline Transaction GetTransaction(int idx) const { return transactions[idx]; }
};