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

void PrintUserInfo(User user)
{
    std::cout << std::endl;
    std::cout << "Name: " << user.GetName() << std::endl;
    std::cout << "Public Key: " << user.GetPublicKey() << std::endl;
    std::cout << "Balance: " << user.GetBalance() << std::endl;
}

void PrintTransactionInfo(Transaction transaction)
{
    std::cout << std::endl;
    std::cout << "TXID: " << transaction.GetTransactionId() << std::endl;
    std::cout << "Sender Key: " << transaction.GetSenderKey() << std::endl;
    std::cout << "Receiver Key: " << transaction.GetReceiverKey() << std::endl;
    std::cout << "Value: " << transaction.GetValue() << std::endl;
}

int main()
{
    std::vector<User> users;
    std::vector<Transaction> transactions;
    GenerateUsers(users, 1000);
    GenerateTransactions(transactions, users, 10000);
    // PrintAllUsers(users);
    // PrintTransactionInfo(transactions[0]);
    // PrintUserInfo(users[2]);
    return 0;
}