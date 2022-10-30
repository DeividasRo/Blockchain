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
    bool confirmed;

public:
    Transaction() : transaction_id(""), sender_key(""), receiver_key(""), value(0), confirmed(false) {}

    Transaction(string transaction_id, string sender_key, string receiver_key, int value)
    {
        this->transaction_id = transaction_id;
        this->sender_key = sender_key;
        this->receiver_key = receiver_key;
        this->value = value;
        this->confirmed = false;
    }

    friend std::istream &operator>>(std::istream &in, Transaction &t)
    {
        in >> t.transaction_id >> t.sender_key >> t.receiver_key >> t.value;
        return in;
    }

    inline void Confirm() { this->confirmed = true; };
    inline bool IsConfirmed() { return confirmed; };

    // setters
    inline void SetTransactionId(string transaction_id) { this->transaction_id = transaction_id; }
    inline void SetSenderKey(string sender_key) { this->sender_key = sender_key; }
    inline void SetReceiverKey(string receiver_key) { this->receiver_key = receiver_key; }
    inline void SetValue(int value) { this->value = value; }

    // getters
    inline string GetTransactionId() const { return transaction_id; }
    inline string GetSenderKey() const { return sender_key; }
    inline string GetReceiverKey() const { return receiver_key; }
    inline int GetValue() const { return value; }
};