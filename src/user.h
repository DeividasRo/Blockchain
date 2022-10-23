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
    unsigned int total_requested_send_value = 0; // Total sent amount of value in a transaction that is waiting to be put into a block

public:
    User() : name(""), public_key(""), balance(0), total_requested_send_value(0) {}

    User(string name, string public_key, unsigned int balance)
    {
        this->name = name;
        this->public_key = public_key;
        this->balance = balance;
    }

    inline void UpdateBalance(int value) { this->balance += value; }
    inline void UpdateTotalRequestedSendValue(int value) { this->total_requested_send_value += value; }
    inline void SetTotalRequestedSendValue(int value) { this->total_requested_send_value = value; }
    // getters
    inline string GetName() const { return name; }
    inline string GetPublicKey() const { return public_key; }
    inline unsigned int GetBalance() const { return balance; }
    inline unsigned int GetTotalRequestedSendValue() const { return total_requested_send_value; }
};