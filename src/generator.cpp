#include "generator.h"

std::random_device rd;
std::mt19937 gen(rd());

int GenerateIntValue(int min_val, int max_val)
{
    std::uniform_int_distribution<> dist(min_val, max_val);
    return dist(gen);
}

void GenerateUsers(std::vector<User> &users, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        string user_name = "user" + std::to_string(users.size() + 1);
        User user = User(user_name, Hash(user_name), GenerateIntValue(100, 100000));
        users.push_back(user);
        // std::cout << "Generated " << user_name << std::endl;
    }
}

void GenerateTransactions(std::vector<Transaction> &transactions, std::vector<User> &users, int amount)
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
        transactions.push_back(transaction);
        // std::cout << "Generated TXID: " << transaction_id << std::endl;
        i++;
    }
}