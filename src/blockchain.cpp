#include "blockchain.h"

string IntToHexString(unsigned int value)
{
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
}

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
        buffer << block.GetCurrentBlockHash() << " " << block.GetPreviousBlockHash() << " " << block.GetMerkelRootHash() << " " << block.GetTimestamp() << " " << block.GetNonce() << " " << block.GetDifficultyTarget() << " " << block.GetHeight() << " " << block.GetVersion() << " " << block.TransactionCount();
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
        if (sender == receiver)
            continue;
        if (sender->GetTotalUnconfirmedSendValue() > sender->GetBalance())
            continue;
        // Amount of money that is available to send (Balance - Sent Unconfirmed Amount)
        unsigned int available_money = sender->GetBalance() - sender->GetTotalUnconfirmedSendValue();

        // Prevents generation of sending value that is bigger than (Available Money / Reducer Value)
        int reducer_value = (available_money > 3) ? 3 : 1;
        unsigned int value = GenerateIntValue(0, available_money / reducer_value);

        sender->UpdateTotalUnconfirmedSendValue(value);

        string transaction_id = Hash(Hash(sender->GetPublicKey() + receiver->GetPublicKey() + std::to_string(value)));

        // Create new transaction
        Transaction transaction = Transaction(transaction_id, sender->GetPublicKey(), receiver->GetPublicKey(), value);

        // Push newly created transaction to transaction pool
        transaction_pool.push_back(transaction);
        i++;
    }
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

    string merkle_hash = Hash(transaction_ids);
    string previous_block_hash;
    if (blocks.size() > 0)
        previous_block_hash = blocks.back().GetCurrentBlockHash();
    else
        previous_block_hash.assign(64, '0');

    unsigned int timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

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
        sender->UpdateTotalUnconfirmedSendValue(-block_transactions[i].GetValue());
        block_transactions[i].Confirm();
    }

    // Construct a new block
    Block block(current_block_hash, previous_block_hash, merkle_hash, timestamp, nonce, difficulty_target, BlockCount(), version);
    block.SetTransactions(block_transactions);

    // And new block to blockchain
    blocks.push_back(block);
}