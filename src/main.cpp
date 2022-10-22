#include "user.h"
#include "generator.h"
#include "block.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

void PrintAllUsers(std::vector<User> users)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i].GetName() << " " << users[i].GetPublicKey() << " " << users[i].GetBalance() << std::endl;
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

void CreateBlock(std::vector<Block> &blocks, std::vector<Transaction> &transactions, int difficulty_target, int version = 1)
{
    // Randomly select 100 transactions from transaction pool
    std::vector<Transaction> block_transactions;
    string transaction_ids = "";
    for (int i = 0; i < 100; i++)
    {
        int transaction_idx = GenerateIntValue(0, transactions.size());
        transactions.erase(transactions.begin() + transaction_idx);
        block_transactions.push_back(transactions[transaction_idx]);
        transaction_ids += transactions[transaction_idx].GetTransactionId();
    }
    transactions.shrink_to_fit();

    unsigned int timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    string merkle_hash = Hash(transaction_ids);
    string previous_block_hash;
    if (blocks.size() > 0)
        previous_block_hash = blocks.back().GetCurrentBlockHash();
    else
        previous_block_hash.assign(64, '0');

    string block_data = IntToHexString(version) + previous_block_hash + merkle_hash + IntToHexString(timestamp) + IntToHexString(timestamp);

    string target_substr(difficulty_target, '0');
    string current_block_hash;
    unsigned int nonce = -1;

    while (current_block_hash.substr(0, difficulty_target) != target_substr)
    {
        nonce++;
        current_block_hash = Hash(block_data + IntToHexString(nonce));
    }
    std::cout << nonce << std::endl;

    Block block(current_block_hash, previous_block_hash, merkle_hash, timestamp, nonce, difficulty_target, version);
    blocks.push_back(block);
}

int main()
{
    std::vector<User> users;
    std::vector<Transaction> transactions;
    std::vector<Block> blocks;
    GenerateUsers(users, 1000);
    GenerateTransactions(transactions, users, 10000);
    // PrintAllUsers(users);
    // PrintTransactionInfo(transactions[0]);
    // PrintUserInfo(users[0]);
    CreateBlock(blocks, transactions, 4);
    // PrintBlockInfo(blocks[0]);
    return 0;
}