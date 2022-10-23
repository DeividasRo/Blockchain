#pragma once
#include <string>
#include <iostream>

using std::string;

class User
{
private:
    string name;
    string public_key;
    int balance;

public:
    User() : name(""), public_key(""), balance(0) {}

    User(string name, string public_key, int balance)
    {
        this->name = name;
        this->public_key = public_key;
        this->balance = balance;
    }

    inline void UpdateBalance(int value) { this->balance += value; }

    // getters
    inline string GetName() const { return name; }
    inline string GetPublicKey() const { return public_key; }
    inline int GetBalance() const { return balance; }
};