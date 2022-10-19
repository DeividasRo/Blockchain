#pragma once
#include <string>

using std::string;

class User
{
private:
    string name_;
    int balance_;
    string public_key_;

public:
    User() : name_(""), public_key_(""), balance_(0) {}

    User(string name, string public_key, int balance)
    {
        name_ = name;
        public_key_ = public_key;
        balance_ = balance;
    }

    // getters
    inline string GetName() const { return name_; }
    inline string GetPublicKey() { return public_key_; }
    inline int GetBalance() const { return balance_; }
};
