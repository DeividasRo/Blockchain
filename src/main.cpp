#include "user.h"
#include "generator.h"
#include "block.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>

void PrintAllUsers(std::vector<User> users)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i].GetName() << " " << users[i].GetPublicKey() << " " << users[i].GetBalance() << " " << users[i].GetTotalRequestedSendValue() << std::endl;
    }
}

void PrintUserInfo(User user)
{
    std::cout << std::endl;
    std::cout << "Name: " << user.GetName() << std::endl;
    std::cout << "Public Key: " << user.GetPublicKey() << std::endl;
    std::cout << "Balance: " << user.GetBalance() << std::endl;
}

void PrintTransactionInfo(Transaction transaction)
{
    std::cout << std::endl;
    std::cout << "TXID: " << transaction.GetTransactionId() << std::endl;
    std::cout << "Sender Key: " << transaction.GetSenderKey() << std::endl;
    std::cout << "Receiver Key: " << transaction.GetReceiverKey() << std::endl;
    std::cout << "Value: " << transaction.GetValue() << std::endl;
}

void PrintBlockInfo(Block block)
{
    std::cout << std::endl;
    std::cout << "Current Block Hash: " << block.GetCurrentBlockHash() << std::endl;
    std::cout << "Previous Block Hash: " << block.GetPreviousBlockHash() << std::endl;
    std::cout << "Merkel Root Hash: " << block.GetMerkelRootHash() << std::endl;
    std::cout << "Timestamp: " << block.GetTimestamp() << std::endl;
    std::cout << "Nonce: " << block.GetNonce() << std::endl;
    std::cout << "Difficulty Target: " << block.GetDifficultyTarget() << std::endl;
    std::cout << "Version: " << block.GetVersion() << std::endl;
}

string IntToHexString(unsigned int value)
{
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
}

void CreateBlock(std::vector<Block> &blocks, std::vector<Transaction> &transactions, std::vector<User> &users, int difficulty_target, int version = 1)
{
    // Randomly select 100 transactions from transaction pool
    std::vector<Transaction> block_transactions;
    string transaction_ids = "";
    for (int i = 0; i < 100; i++)
    {
        int transaction_idx = GenerateIntValue(0, transactions.size());
        block_transactions.push_back(transactions.front());
        transaction_ids += transactions[transaction_idx].GetTransactionId();
        transactions.erase(transactions.begin());
        if (transactions.size() == 0)
            break;
    }
    transactions.shrink_to_fit();

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

    PrintAllUsers(users);

    // Construct a new block
    Block block(current_block_hash, previous_block_hash, merkle_hash, timestamp, nonce, difficulty_target, version);
    block.SetTransactions(block_transactions);

    // And new block to blockchain
    blocks.push_back(block);
}

int main()
{
    std::vector<User> users;
    std::vector<Transaction> transactions;
    std::vector<Block> blocks;
    int difficulty = 4;
    GenerateUsers(users, 100);
    GenerateTransactions(transactions, users, 10000);
    std::cout << transactions.size() << std::endl;
    // PrintAllUsers(users);
    // PrintTransactionInfo(transactions[0]);
    // PrintUserInfo(users[0]);
    while (transactions.size() > 0)
    {
        CreateBlock(blocks, transactions, users, difficulty);
        PrintBlockInfo(blocks[blocks.size() - 1]);
    }
    return 0;
}

// Difficulty 7 apie 8 minutes