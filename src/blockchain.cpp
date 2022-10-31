#include "blockchain.h"

void Blockchain::SaveUsersData()
{
    std::ofstream outf("data/users.txt");
    std::stringstream buffer;
    bool first = true;
    for (auto user : users)
    {
        if (!first)
            buffer << "\n";
        buffer << user.GetName() << " " << user.GetPublicKey() << " " << user.GetBalance() << " " << user.GetTotalUnconfirmedSendValue();
        first = false;
    }
    outf << buffer.str();
    buffer.clear();
    outf.close();
}

void Blockchain::SaveTransactionPoolData()
{
    std::ofstream outf("data/txpool.txt");
    std::stringstream buffer;
    bool first = true;
    for (auto transaction : transaction_pool)
    {
        if (!first)
            buffer << "\n";
        buffer << transaction.GetTransactionId() << " " << transaction.GetSenderKey() << " " << transaction.GetReceiverKey() << " " << transaction.GetValue();
        first = false;
    }
    outf << buffer.str();
    buffer.clear();
    outf.close();
}

void Blockchain::SaveBlocksData()
{
    std::ofstream outf("data/blocks.txt");
    std::stringstream buffer;
    bool first = true;
    for (auto block : blocks)
    {
        if (!first)
            buffer << "\n";
        buffer << block.GetCurrentBlockHash() << " " << block.GetPreviousBlockHash() << " " << block.GetMerkelRootHash() << " " << block.GetMiner() << " " << block.GetTimestamp() << " " << block.GetNonce() << " " << block.GetDifficultyTarget() << " " << block.GetHeight() << " " << block.GetVersion() << " " << block.TransactionCount();
        for (auto transaction : block.GetTransactions())
        {
            buffer << "\n"
                   << transaction.GetTransactionId() << " " << transaction.GetSenderKey() << " " << transaction.GetReceiverKey() << " " << transaction.GetValue();
        }
        first = false;
    }
    outf << buffer.str();
    buffer.clear();
    outf.close();
}

void Blockchain::LoadUsersData()
{
    std::ifstream inf("data/users.txt");
    if (inf.peek() == std::ifstream::traits_type::eof())
        return;
    std::stringstream buffer;
    buffer << inf.rdbuf();
    inf.close();
    while (!buffer.eof())
    {
        User user;
        buffer >> user;
        users.push_back(user);
    }
    buffer.clear();
}

void Blockchain::LoadTransactionPoolData()
{
    std::ifstream inf("data/txpool.txt");
    if (inf.peek() == std::ifstream::traits_type::eof())
        return;
    std::stringstream buffer;
    buffer << inf.rdbuf();
    inf.close();
    while (!buffer.eof())
    {
        Transaction transaction;
        buffer >> transaction;
        transaction_pool.push_back(transaction);
    }
    buffer.clear();
}

void Blockchain::LoadBlocksData()
{
    std::ifstream inf("data/blocks.txt");
    if (inf.peek() == std::ifstream::traits_type::eof())
        return;
    std::stringstream buffer;
    buffer << inf.rdbuf();
    inf.close();
    while (!buffer.eof())
    {
        Block block;
        buffer >> block;
        blocks.push_back(block);
    }
    buffer.clear();
}

void Blockchain::GenerateUsers(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        string user_name = "user" + std::to_string(users.size() + 1);
        User user = User(user_name, Hash(user_name), GenerateIntValue(100, 100000));
        users.push_back(user);
    }
}

void Blockchain::GenerateTransactionPool(int amount)
{
    int i = 0;
    while (i < amount)
    {
        User *sender = &users[GenerateIntValue(0, users.size() - 1)];

        User *receiver = &users[GenerateIntValue(0, users.size() - 1)];

        // Checks if sender and receiver are different users
        if (sender == receiver)
            continue;
        // Checks if sender's balance is greater than total pending send value
        if (sender->GetTotalUnconfirmedSendValue() > sender->GetBalance())
            continue;

        // Amount of money that is available to send (Balance - Sent Unconfirmed Amount)
        unsigned int available_money = sender->GetBalance() - sender->GetTotalUnconfirmedSendValue();

        // Prevents generation of sending value that is bigger than (Available Money / Reducer Value)
        int reducer_value = (available_money > 4) ? 4 : 1;
        unsigned int value = GenerateIntValue(0, available_money / reducer_value);

        sender->UpdateTotalUnconfirmedSendValue(value);

        // Timestamp of transaction creation
        unsigned int timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        // Make transaction id out of transaction data
        string transaction_id = Hash(Hash(sender->GetPublicKey() + receiver->GetPublicKey() + IntToHexString(timestamp) + std::to_string(value)));

        // Create new transaction
        Transaction transaction = Transaction(transaction_id, sender->GetPublicKey(), receiver->GetPublicKey(), timestamp, value);

        // If transaction is validated it is added to the transaction pool
        if (transaction.GetTransactionId() == Hash(Hash(transaction.GetSenderKey() + transaction.GetReceiverKey() + IntToHexString(transaction.GetTimestamp()) + std::to_string(transaction.GetValue()))))
            transaction_pool.push_back(transaction);
        i++;
    }
}

string MerkleRootHash(std::vector<string> merkle)
{
    while (merkle.size() > 1)
    {
        if (merkle.size() % 2 != 0)
            merkle.push_back(merkle.back());
        std::vector<string> new_merkle;
        for (auto it = merkle.begin(); it != merkle.end(); it += 2)
        {
            new_merkle.push_back(Hash(*it + *(it + 1)));
        }
        merkle = new_merkle;
    }
    return merkle[0];
}

void Blockchain::CreateBlock()
{
    unsigned long long nonce_target = 100000;
    const int miner_count = 5;
    while (nonce_target < ULLONG_MAX && nonce_target > 0)
    {
        std::vector<string> miners = {"1A", "1B", "1C", "1D", "1E"};
        for (int i = 0; i < miner_count; i++)
        {
            int miner_idx = GenerateIntValue(0, miners.size());
            if (MineBlock(nonce_target, miners[miner_idx]))
                return;
            miners.erase(miners.begin() + miner_idx);
        }
        nonce_target += 100000;
    }
    return;
}

bool Blockchain::MineBlock(unsigned long long nonce_target, string miner_name)
{
    // Randomly assign 100 transactions for 5 different miners from transaction pool
    std::vector<Transaction> block_transactions;
    int transaction_idxs[100];
    std::vector<string> transaction_ids;
    for (int i = 0; i < 100; i++)
    {
        transaction_idxs[i] = GenerateIntValue(0, transaction_pool.size() - 1);
        block_transactions.push_back(transaction_pool[transaction_idxs[i]]);
        transaction_ids.push_back(transaction_pool[transaction_idxs[i]].GetTransactionId());
        if (block_transactions.size() == transaction_pool.size())
            break;
    }
    transaction_pool.shrink_to_fit();

    string merkle_root_hash = MerkleRootHash(transaction_ids);

    string previous_block_hash;
    if (blocks.size() > 0)
        previous_block_hash = blocks.back().GetCurrentBlockHash();
    else
        previous_block_hash.assign(64, '0');

    unsigned int timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    string target_substr(difficulty_target, '0');
    string current_block_hash;
    unsigned int nonce = -1;

    // Mining
    while (current_block_hash.substr(0, difficulty_target) != target_substr)
    {
        nonce++;
        if (nonce > nonce_target)
            return false;
        current_block_hash = Hash(IntToHexString(version) + previous_block_hash + merkle_root_hash + IntToHexString(timestamp) + IntToHexString(nonce) + IntToHexString(difficulty_target));
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
        sender->UpdateTotalUnconfirmedSendValue(-block_transactions[i].GetValue());
        block_transactions[i].SetTimestamp(timestamp);
        block_transactions[i].Confirm();

        transaction_pool.erase(transaction_pool.begin() + transaction_idxs[i]);
    }

    // Construct a new block
    Block block(current_block_hash, previous_block_hash, merkle_root_hash, miner_name, timestamp, nonce, difficulty_target, BlockCount(), version, block_transactions);

    // And new block to blockchain
    blocks.push_back(block);

    return true;
}