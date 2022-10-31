#include "blockchain.h"
#include "helpers.h"

void PrintAllUsers(std::vector<User> users)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << "N: " << users[i].GetName()
                  << "  PK: " << users[i].GetPublicKey()
                  << "  B: " << users[i].GetBalance()
                  << "  TUSV: " << users[i].GetTotalUnconfirmedSendValue()
                  << std::endl;
    }
}

void PrintUserInfo(User user)
{
    std::cout << "Name: " << user.GetName() << std::endl;
    std::cout << "Public Key: " << user.GetPublicKey() << std::endl;
    std::cout << "Balance: " << user.GetBalance() << std::endl;
    std::cout << std::endl;
}

void PrintTransactionInfo(Transaction transaction)
{
    std::cout << "Status: " << ((transaction.IsConfirmed()) ? "Confirmed" : "Unconfirmed") << std::endl;
    std::cout << "TXID: " << transaction.GetTransactionId() << std::endl;
    std::cout << "Sender Key: " << transaction.GetSenderKey() << std::endl;
    std::cout << "Receiver Key: " << transaction.GetReceiverKey() << std::endl;
    std::cout << "Timestamp: " << TimeStampToHReadable(transaction.GetTimestamp()) << std::endl;
    std::cout << "Value: " << transaction.GetValue() << std::endl;
    std::cout << std::endl;
}

void PrintBlockInfo(Block block)
{
    std::cout << "BLOCK #" << block.GetHeight() << std::endl;
    std::cout << "Current Block Hash: " << block.GetCurrentBlockHash() << std::endl;
    std::cout << "Previous Block Hash: " << block.GetPreviousBlockHash() << std::endl;
    std::cout << "Merkel Root Hash: " << block.GetMerkelRootHash() << std::endl;
    std::cout << "Miner: " << block.GetMiner() << std::endl;
    std::cout << "Timestamp: " << TimeStampToHReadable(block.GetTimestamp());
    std::cout << "Nonce: " << block.GetNonce() << std::endl;
    std::cout << "Difficulty Target: " << block.GetDifficultyTarget() << std::endl;
    std::cout << "Transaction Count: " << block.TransactionCount() << std::endl;
    std::cout << "Version: " << block.GetVersion() << std::endl;
    std::cout << "Height: " << block.GetHeight() << std::endl;
    std::cout << std::endl;
}

int main()
{
    string input;
    int argc;
    std::vector<string> args;
    Blockchain blockchain;

    blockchain.SetDifficultyTarget(5);
    blockchain.SetVersion(1);

    if (file_exists("data/users.txt"))
        blockchain.LoadUsersData();
    if (file_exists("data/txpool.txt"))
        blockchain.LoadTransactionPoolData();
    if (file_exists("data/blocks.txt"))
        blockchain.LoadBlocksData();

    std::cout << "BLOCKCHAIN INITIATED" << std::endl;
    do
    {
        std::cout << ">";
        // Get command input
        std::getline(std::cin, input);
        // Input to lower case
        std::for_each(input.begin(), input.end(), [](char &c)
                      { c = ::tolower(c); });
        // Split input into arguments
        args = split(input);
        // Argument count
        argc = args.size();

        // Commands
        if (args[0] == "genusers")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 1)
                    continue;
                blockchain.GenerateUsers(std::stoi(args[1]));
                std::cout << "Generated " << std::stoi(args[1]) << " new users." << std::endl;
                std::cout << "Currently there are " << blockchain.UserCount() << " users." << std::endl;

                blockchain.SaveUsersData();
            }
            else
            {
                std::cout << "Invalid amount of arguments." << std::endl;
            }
        }
        else if (args[0] == "gentx")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 1)
                    continue;
                if (blockchain.UserCount() < 2)
                {
                    std::cout << "Not enough users in the blockchain." << std::endl;
                    continue;
                }
                int pre_gen_pool_size = blockchain.TransactionPoolCount();
                std::cout << "Attempting to generate " << std::stoi(args[1]) << " new transactions..." << std::endl;
                blockchain.GenerateTransactionPool(std::stoi(args[1]));
                std::cout << "Generated " << blockchain.TransactionPoolCount() - pre_gen_pool_size << " new transactions in transaction pool." << std::endl;
                std::cout << "Currently there are " << blockchain.TransactionPoolCount() << " transactions in transaction pool." << std::endl;

                blockchain.SaveUsersData();
                blockchain.SaveTransactionPoolData();
            }
            else
            {
                std::cout << "Invalid amount of arguments." << std::endl;
            }
        }
        else if (args[0] == "createblocks")
        {
            if (blockchain.TransactionPoolCount() > 0)
            {
                while (blockchain.TransactionPoolCount() > 0)
                {
                    blockchain.CreateBlock();

                    PrintBlockInfo(blockchain.GetBlock(blockchain.BlockCount() - 1));

                    blockchain.SaveUsersData();
                    blockchain.SaveTransactionPoolData();
                    blockchain.SaveBlocksData();
                }
            }
            else
                std::cout << "No transactions in transaction pool." << std::endl;
        }
        else if (args[0] == "listblocktx")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 0 || std::stoi(args[1]) > blockchain.BlockCount() - 1)
                    continue;
                Block block = blockchain.GetBlock(std::stoi(args[1]));
                for (int i = 0; i < block.TransactionCount(); i++)
                {
                    std::cout << "TXID: " << block.GetTransaction(i).GetTransactionId() << std::endl;
                }
            }
            else
            {
                std::cout << "Invalid amount of arguments." << std::endl;
            }
        }
        else if (args[0] == "listtxpool")
        {
            for (int i = 0; i < blockchain.TransactionPoolCount(); i++)
            {
                std::cout << "TXID: " << blockchain.GetTransactionFromPool(i).GetTransactionId() << std::endl;
            }
        }
        else if (args[0] == "getblockinfo")
        {
            if (argc == 2)
            {
                if (!is_number(args[1]))
                    continue;
                if (std::stoi(args[1]) < 0 || std::stoi(args[1]) > blockchain.BlockCount() - 1)
                    continue;
                PrintBlockInfo(blockchain.GetBlock(std::stoi(args[1])));
            }
            else
            {
                std::cout << "Invalid amount of arguments." << std::endl;
            }
        }
        else if (args[0] == "getuserinfo")
        {
            if (argc == 2)
            {
                for (int i = 0; i < blockchain.UserCount(); i++)
                {
                    if (blockchain.GetUsers()[i].GetPublicKey() == args[1])
                    {
                        PrintUserInfo(blockchain.GetUsers()[i]);
                        break;
                    }
                }
            }
            else
            {
                std::cout << "Invalid amount of arguments." << std::endl;
            }
        }
        else if (args[0] == "gettxinfo")
        {
            if (argc == 2)
            {
                std::vector<Transaction> transactions = blockchain.GetTransactionPool();
                auto transaction = std::find_if(std::begin(transactions), std::end(transactions),
                                                [&](Transaction const &t)
                                                { return t.GetTransactionId() == args[1]; });
                if (transaction != transactions.end())
                {
                    PrintTransactionInfo(*transaction);
                }
                else
                {
                    for (int i = 0; i < blockchain.BlockCount(); i++)
                    {
                        Block block = blockchain.GetBlock(i);
                        transactions = block.GetTransactions();
                        auto transaction = std::find_if(std::begin(transactions), std::end(transactions),
                                                        [&](Transaction const &t)
                                                        { return t.GetTransactionId() == args[1]; });
                        if (transaction != transactions.end())
                        {
                            PrintTransactionInfo(*transaction);
                            break;
                        }
                    }
                }
                transactions.clear();
                transactions.shrink_to_fit();
            }
            else
            {
                std::cout << "Invalid amount of arguments." << std::endl;
            }
        }
        else if (args[0] == "getusercount")
        {

            std::cout << "Currently there are " << blockchain.UserCount() << " users (wallets) in the blockchain." << std::endl;
        }
        else if (args[0] == "getblockcount")
        {
            std::cout << "Currently there are " << blockchain.BlockCount() << " blocks in the blockchain." << std::endl;
        }
        else if (args[0] == "gettxpoolsize")
        {
            std::cout << "Currently there are " << blockchain.TransactionPoolCount() << " unconfirmed transactions in the transaction pool." << std::endl;
        }
        else if (args[0] == "reset")
        {
            int status = remove("data/users.txt");
            status = remove("data/txpool.txt");
            status = remove("data/blocks.txt");
            blockchain.ClearBlockchain();
            std::cout << "Blockchain data cleared successfully." << std::endl;
        }
        else if (args[0] == "help")
        {
            std::cout << "GENUSERS <amount> - generate a specified amount of blockchain users (wallets)." << std::endl;
            std::cout << "GENTX <amount> - generate a specified amount of unconfirmed transactions in the transaction pool." << std::endl;
            std::cout << "CREATEBLOCKS - initiate block mining until all transactions are confirmed." << std::endl;
            std::cout << "LISTBLOCKTX <block-height> - list all transaction information of a specified block." << std::endl;
            std::cout << "LISTTXPOOL - list all real time transaction information in the transaction pool." << std::endl;
            std::cout << "GETUSERINFO <public-key> - display information about a specified user." << std::endl;
            std::cout << "GETTXINFO <txid> - display information about a specified transaction" << std::endl;
            std::cout << "GETBLOCKINFO <block-height> - display information about a specified block." << std::endl;
            std::cout << "GETUSERCOUNT - display a real time amount of users (wallets) in the blockchain." << std::endl;
            std::cout << "GETBLOCKCOUNT - display a real time amount of blocks in the blockchain." << std::endl;
            std::cout << "GETTXPOOLSIZE - display a real time amount of unconfirmed transactions in the transaction pool." << std::endl;
            std::cout << "RESET - reset blockchain, clearing all of its data." << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Invalid command." << std::endl;
        }
    } while (args[0] != "stop");
    return 0;
}

// Difficulty 6 takes around 4 minutes
// Difficulty 7 takes around 8 minutes