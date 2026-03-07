#pragma once
#include <../shared/classes/UserAccount.hpp>

class UserOperations
{
    int sockfd;
    public:
    UserOperations(int sockfd);
    void check_balance(UserAccount user);
    void deposit(UserAccount user);
    void withdraw(UserAccount user);
    void transfer_money(UserAccount user);
    void transaction_history(UserAccount user);
    void logout(UserAccount user);
};

void useroperation_init(int passed_sockfd);