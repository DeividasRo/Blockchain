#pragma once
#include <string>
#include <iostream>

using std::string;

class User
{
private:
    string name;
    string public_key;
    unsigned int balance = 0;
    unsigned int total_unconfirmed_send_value = 0; // Total sent amount of value in a transaction that is waiting to be put into a block

public:
    User() : name(""), public_key(""), balance(0), total_unconfirmed_send_value(0) {}

    User(string name, string public_key, unsigned int balance)
    {
        this->name = name;
        this->public_key = public_key;
        this->balance = balance;
    }

    inline void UpdateBalance(int value) { this->balance += value; }
    inline void UpdateTotalUnconfirmedSendValue(int value) { this->total_unconfirmed_send_value += value; }
    inline void SetTotalUnconfirmedSendValue(int value) { this->total_unconfirmed_send_value = value; }
    // getters
    inline string GetName() const { return name; }
    inline string GetPublicKey() const { return public_key; }
    inline unsigned int GetBalance() const { return balance; }
    inline unsigned int GetTotalUnconfirmedSendValue() const { return total_unconfirmed_send_value; }
};