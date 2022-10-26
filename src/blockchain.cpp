#include "blockchain.h"

string IntToHexString(unsigned int value)
{
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
}

void Blockchain::CreateBlock(int difficulty_target, int version)
{
    // Randomly select 100 transactions from transaction pool
    std::vector<Transaction> block_transactions;
    string transaction_ids = "";
    for (int i = 0; i < 100; i++)
    {
        int transaction_idx = GenerateIntValue(0, transaction_pool.size() - 1);
        block_transactions.push_back(transaction_pool[transaction_idx]);
        transaction_ids += transaction_pool[transaction_idx].GetTransactionId();
        transaction_pool.erase(transaction_pool.begin() + transaction_idx);
        if (transaction_pool.size() == 0)
            break;
    }
    transaction_pool.shrink_to_fit();

    unsigned int timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    string merkle_hash = Hash(transaction_ids);
    string previous_block_hash;
    if (blocks.size() > 0)
        previous_block_hash = blocks.back().GetCurrentBlockHash();
    else
        previous_block_hash.assign(64, '0');

    string block_data = IntToHexString(version) + previous_block_hash + merkle_hash + IntToHexString(timestamp) + IntToHexString(difficulty_target);

    string target_substr(difficulty_target, '0');
    string current_block_hash;
    unsigned int nonce = -1;

    // Mining
    while (current_block_hash.substr(0, difficulty_target) != target_substr)
    {
        nonce++;
        current_block_hash = Hash(block_data + IntToHexString(nonce));
    }

    // Complete transactions
    for (int i = 0; i < block_transactions.size(); i++)
    {

        auto sender = std::find_if(std::begin(users), std::end(users),
                                   [&](User const &u)
                                   { return u.GetPublicKey() == block_transactions[i].GetSenderKey(); });

        auto receiver = std::find_if(std::begin(users), std::end(users),
                                     [&](User const &u)
                                     { return u.GetPublicKey() == block_transactions[i].GetReceiverKey(); });
        receiver->UpdateBalance(block_transactions[i].GetValue());
        sender->UpdateBalance(-block_transactions[i].GetValue());
        sender->UpdateTotalRequestedSendValue(-block_transactions[i].GetValue());
    }

    // Construct a new block
    Block block(current_block_hash, previous_block_hash, merkle_hash, timestamp, nonce, difficulty_target, BlockCount(), version);
    block.SetTransactions(block_transactions);

    // And new block to blockchain
    blocks.push_back(block);
}