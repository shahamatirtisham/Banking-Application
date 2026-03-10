#pragma once
#include <../shared/classes/UserAccount.hpp>

class UserOperations
{
    int sockfd;
    public:
    UserOperations(int sockfd);
    void check_balance(UserAccount& user);
    int deposit(UserAccount& user);
    int withdraw(UserAccount& user);
    int transfer_money(UserAccount& user);
    void transaction_history(const UserAccount& user);
    void logout(const UserAccount& user);
};

