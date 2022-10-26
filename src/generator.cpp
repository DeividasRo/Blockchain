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
        string user_name = "user" + std::to_string(i + 1);
        User user = User(user_name, Hash(user_name), GenerateIntValue(100, 100000));
        users.push_back(user);
    }
}

void GenerateTransactions(std::vector<Transaction> &transactions, std::vector<User> &users, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        User *sender = &users[GenerateIntValue(0, users.size() - 1)];
        User *receiver = &users[GenerateIntValue(0, users.size() - 1)];
        if (sender->GetTotalRequestedSendValue() >= sender->GetBalance())
            continue;
        if (sender->GetBalance() > sender->GetTotalRequestedSendValue() && sender != receiver)
        {
            // Generate value to send from (1) to (Available Money / 5)
            int value = GenerateIntValue(1, (sender->GetBalance() - sender->GetTotalRequestedSendValue()) / 10);

            sender->UpdateTotalRequestedSendValue(value);

            string transaction_id = Hash(Hash(sender->GetPublicKey() + receiver->GetPublicKey() + std::to_string(value)));

            // Create new transaction
            Transaction transaction = Transaction(transaction_id, sender->GetPublicKey(), receiver->GetPublicKey(), value);
            // Push newly created transaction to transaction pool
            transactions.push_back(transaction);
        }
    }
}