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
        string user_name = "user" + std::to_string(i);
        User user = User(user_name, Hash(user_name), GenerateIntValue(100, 1000000));
        users.push_back(user);
    }
}