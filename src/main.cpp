#include "blockchain.h"
#include <iostream>
#include <iomanip>

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
    std::cout << "Height: " << block.GetHeight() << std::endl;
}

int main()
{
    Blockchain blockchain("MyCoin");
    int difficulty = 4;

    blockchain.GenerateBlockchainUsers(100);
    std::cout << "Generated " << blockchain.UserCount() << " users." << std::endl;
    blockchain.GenerateUnverifiedTransactions(10000);
    std::cout << "Generated " << blockchain.TransactionCount() << " unverified users." << std::endl;
    // PrintAllUsers(users);
    // PrintTransactionInfo(transactions[0]);
    // PrintUserInfo(users[0]);

    std::cout << "\nCreating blocks for all unverified transactions..." << blockchain.BlockCount() << ":" << std::endl;
    while (blockchain.TransactionCount() > 0)
    {
        blockchain.CreateBlock(difficulty);
        PrintBlockInfo(blockchain.GetBlocks()[blockchain.BlockCount() - 1]);
    }
    return 0;
}

// Difficulty 7 apie 8 minutes