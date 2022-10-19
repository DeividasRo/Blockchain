#pragma once
#include <string>

using std::string;

class Transaction
{
private:
    string transaction_id_;
    string sender_key_;
    string receiver_key_;
    int amount_ = 0;

public:
    Transaction() : transaction_id_(""), sender_key_(""), receiver_key_(""), amount_(0) {}

    // getters
    inline string GetTransactionId() const { return transaction_id_; }
    inline string GetSenderKey() { return sender_key_; }
    inline string GetReceiverKey() const { return receiver_key_; }
    inline int GetAmount() const { return amount_; }
};