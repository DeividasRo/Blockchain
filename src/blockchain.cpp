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

void Blockchain::MineBlock(string miner_name, int initial_block_count)
{
    // std::cout << miner_name << std::endl;
    std::vector<Transaction> temp_transaction_pool = transaction_pool;
    std::vector<Transaction> block_transactions;
    std::vector<string> transaction_ids;

    // Selecting block transactions
    for (int i = 0; i < 100; i++)
    {
        int transaction_idx = GenerateIntValue(0, temp_transaction_pool.size() - 1);
        block_transactions.push_back(temp_transaction_pool[transaction_idx]);
        transaction_ids.push_back(temp_transaction_pool[transaction_idx].GetTransactionId());
        temp_transaction_pool.erase(temp_transaction_pool.begin() + transaction_idx);
        if (block_transactions.size() == transaction_pool.size())
            break;
    }
    temp_transaction_pool.shrink_to_fit();

    string merkle_root_hash = MerkleRootHash(transaction_ids);

    string previous_block_hash;
    if (blocks.size() > 0)
        previous_block_hash = blocks.back().GetCurrentBlockHash();
    else
        previous_block_hash.assign(64, '0');

    unsigned int timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    string target_substr(difficulty_target, '0');
    string current_block_hash;
    unsigned long long nonce = -1;

    // Mining
    while (current_block_hash.substr(0, difficulty_target) != target_substr)
    {
        if (initial_block_count != BlockCount()) // Checks if the block was already mined
            return;
        nonce++;
        current_block_hash = Hash(IntToHexString(version) + previous_block_hash + merkle_root_hash + IntToHexString(timestamp) + IntToHexString(nonce) + IntToHexString(difficulty_target));
        if (nonce == ULLONG_MAX)
            nonce = 0;
    }

    // Add an empty block to the blockchain
    Block block;
    blocks.push_back(block);

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

        auto t = std::find_if(std::begin(transaction_pool), std::end(transaction_pool),
                              [&](Transaction const &t)
                              { return t.GetTransactionId() == block_transactions[i].GetTransactionId(); });
        transaction_pool.erase(t);
    }
    transaction_pool.shrink_to_fit();

    // Construct the new block added to the blockchain
    blocks.back() = Block(current_block_hash, previous_block_hash, merkle_root_hash, miner_name, timestamp, nonce, difficulty_target, BlockCount() - 1, version, block_transactions);
}