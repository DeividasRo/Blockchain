#include "user.h"
#include "generator.h"
#include <iostream>
#include <vector>

int main()
{
    std::vector<User> users;
    GenerateUsers(users, 1000);

    for (int i = 0; i < users.size(); i++)
    {
        std::cout << users[i].GetName() << " " << users[i].GetPublicKey() << " " << users[i].GetBalance() << std::endl;
    }
    return 0;
}