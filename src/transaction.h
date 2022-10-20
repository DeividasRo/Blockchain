#pragma once
#include <string>

using std::string;

class Transaction
{
private:
    string transaction_id_;
    string sender_key_;
    string receiver_key_;
    int value_ = 0;

public:
    Transaction() : transaction_id_(""), sender_key_(""), receiver_key_(""), value_(0) {}

    Transaction(string transaction_id, string sender_key, string receiver_key, int value)
    {
        transaction_id_ = transaction_id;
        sender_key_ = sender_key;
        receiver_key_ = receiver_key;
        value_ = value;
    }
    // getters
    inline string GetTransactionId() const { return transaction_id_; }
    inline string GetSenderKey() { return sender_key_; }
    inline string GetReceiverKey() const { return receiver_key_; }
    inline int GetValue() const { return value_; }
};