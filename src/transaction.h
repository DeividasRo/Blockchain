#pragma once
#include <string>
#include <iostream>

using std::string;

class Transaction
{
private:
    string transaction_id;
    string sender_key;
    string receiver_key;
    int value = 0;

public:
    Transaction() : transaction_id(""), sender_key(""), receiver_key(""), value(0) {}

    Transaction(string transaction_id, string sender_key, string receiver_key, int value)
    {
        this->transaction_id = transaction_id;
        this->sender_key = sender_key;
        this->receiver_key = receiver_key;
        this->value = value;
    }
    // getters
    inline string GetTransactionId() const { return transaction_id; }
    inline string GetSenderKey() { return sender_key; }
    inline string GetReceiverKey() const { return receiver_key; }
    inline int GetValue() const { return value; }
};