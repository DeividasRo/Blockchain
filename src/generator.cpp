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

void GenerateTransactions(std::vector<Transaction> &transactions, std::vector<User> users, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        string sender_key = users[GenerateIntValue(0, users.size() - 1)].GetPublicKey();
        string receiver_key = users[GenerateIntValue(0, users.size() - 1)].GetPublicKey();
        int value = GenerateIntValue(0, 50000);
        string transaction_id = Hash(Hash(sender_key + receiver_key + std::to_string(value)));
        Transaction transaction = Transaction(transaction_id, sender_key, receiver_key, value);
        transactions.push_back(transaction);
    }
}