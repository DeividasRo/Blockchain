#include "user.h"
#include "generator.h"
#include <iostream>
#include <vector>

void PrintAllUsers(std::vector<User> users)
{
    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i].GetName() << " " << users[i].GetPublicKey() << " " << users[i].GetBalance() << std::endl;
    }
}

int main()
{
    std::vector<User> users;
    GenerateUsers(users, 1000);
    PrintAllUsers(users);
    return 0;
}