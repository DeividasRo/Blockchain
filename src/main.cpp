#include "blockchain.h"
#include <iostream>
#include <iomanip>

void PrintAllUsers(std::vector<User> users)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i].GetName() << " " << users[i].GetPublicKey() << " " << users[i].GetBalance() << " " << users[i].GetTotalUnconfirmedSendValue() << std::endl;
    }
}

void PrintUserInfo(User user)
{
    std::cout << std::endl;
    std::cout << "Name: " << user.GetName() << std::endl;
    std::cout << "Public Key: " << user.GetPublicKey() << std::endl;
    std::cout << "Balance: " << user.GetBalance() << std::endl;
    std::cout << std::endl;
}

void PrintTransactionInfo(Transaction transaction)
{
    std::cout << std::endl;
    std::cout << "TXID: " << transaction.GetTransactionId() << std::endl;
    std::cout << "Sender Key: " << transaction.GetSenderKey() << std::endl;
    std::cout << "Receiver Key: " << transaction.GetReceiverKey() << std::endl;
    std::cout << "Value: " << transaction.GetValue() << std::endl;
    std::cout << std::endl;
}

void PrintBlockInfo(Block block)
{
    std::cout << std::endl;
    std::cout << "BLOCK #" << block.GetHeight() << std::endl;
    std::cout << "Current Block Hash: " << block.GetCurrentBlockHash() << std::endl;
    std::cout << "Previous Block Hash: " << block.GetPreviousBlockHash() << std::endl;
    std::cout << "Merkel Root Hash: " << block.GetMerkelRootHash() << std::endl;
    std::cout << "Timestamp: " << block.GetTimestamp() << std::endl;
    std::cout << "Nonce: " << block.GetNonce() << std::endl;
    std::cout << "Difficulty Target: " << block.GetDifficultyTarget() << std::endl;
    std::cout << "Transaction Count: " << block.TransactionCount() << std::endl;
    std::cout << "Version: " << block.GetVersion() << std::endl;
    std::cout << "Height: " << block.GetHeight() << std::endl;
    std::cout << std::endl;
}

bool is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c)
                                      { return !std::isdigit(c); }) == s.end();
}

std::vector<string> split(string str)
{
    std::vector<string> internal;
    std::istringstream ss(str);

    string word;

    while (ss >> word)
    {
        internal.push_back(word);
    }

    return internal;
}

int main()
{
    Blockchain blockchain("MyCoin");
    int difficulty = 4;
    string input;
    int argc;
    std::vector<string> args;
    std::cout << "BLOCKCHAIN INSTANTIATED" << std::endl;
    do
    {
        std::cout << ">";
        // Get command input
        std::getline(std::cin, input);
        // To lower case
        std::for_each(input.begin(), input.end(), [](char &c)
                      { c = ::tolower(c); });
        // Split input into arguments
        args = split(input);
        // Argument count
        argc = args.size();

        // Command behaviours
        if (args[0] == "genusers")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 1)
                    continue;
                blockchain.GenerateBlockchainUsers(std::stoi(args[1]));
                std::cout << "Generated " << std::stoi(args[1]) << " new users." << std::endl;
                std::cout << "Currently there are " << blockchain.UserCount() << " users." << std::endl;
            }
        }
        else if (args[0] == "gentransactions")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 1)
                    continue;
                int pre_gen_pool_size = blockchain.TransactionPoolCount();
                std::cout << "Attempting to generate " << std::stoi(args[1]) << " new transactions..." << std::endl;
                blockchain.GenerateTransactionPool(std::stoi(args[1]));
                std::cout << "Generated " << blockchain.TransactionPoolCount() - pre_gen_pool_size << " new transactions in transaction pool." << std::endl;
                std::cout << "Currently there are " << blockchain.TransactionPoolCount() << " transactions in transaction pool." << std::endl;
            }
        }
        else if (args[0] == "createblocks")
        {
            if (blockchain.TransactionPoolCount() > 0)
            {
                while (blockchain.TransactionPoolCount() > 0)
                {
                    blockchain.CreateBlock(difficulty);
                    PrintBlockInfo(blockchain.GetBlock(blockchain.BlockCount() - 1));
                }
            }
            else
            {
                std::cout << "No transactions in transaction pool." << std::endl;
            }
        }
        else if (args[0] == "listblocktransactions")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                Block block = blockchain.GetBlock(std::stoi(args[1]));
                if (std::stoi(args[1]) < 0 && std::stoi(args[1]) > blockchain.BlockCount())
                    continue;
                for (int i = 0; i < blockchain.GetBlock(std::stoi(args[1])).TransactionCount(); i++)
                {
                    PrintTransactionInfo(block.GetTransaction(i));
                }
            }
        }
        else if (args[0] == "getblockinfo")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 0 && std::stoi(args[1]) > blockchain.BlockCount())
                    continue;
                PrintBlockInfo(blockchain.GetBlock(std::stoi(args[1])));
            }
        }
        else if (args[0] == "getuserinfo")
        {
            if (argc == 2)
            {
                for (int i = 0; i < blockchain.UserCount(); i++)
                {
                    if (blockchain.GetUsers()[i].GetName() == args[1])
                        PrintUserInfo(blockchain.GetUsers()[i]);
                }
            }
        }
        else if (args[0] == "help")
        {
            std::cout << std::endl;
            std::cout << "GENUSERS <amount> - generate a specified amount of blockchain users (wallets)." << std::endl;
            std::cout << "GENTRANSACTIONS <amount> - generate a specified amount of unconfirmed transactions in transaction pool." << std::endl;
            std::cout << "CREATEBLOCKS - initiate block mining until all transactions are confirmed." << std::endl;
            std::cout << "LISTBLOCKTRANSACTIONS <block_index> - list all transaction information of a specified block." << std::endl;
            std::cout << "GETBLOCKINFO <block_index> - display information about a specified block." << std::endl;
            std::cout << "GETUSERINFO <user_name>- display information about a specified user." << std::endl;
            std::cout << std::endl;
        }
    } while (args[0] != "stop");
    return 0;
}

// TODO:
// Comment all code
// More user commands ()
// More detailed README