#include <../shared/classes/UserAccount.hpp>

class UserOperations
{
    int sockfd;
    public:
    void check_balance(UserAccount user);
    void deposit(UserAccount user);
    void withdraw(UserAccount user);
    void transfer_money(UserAccount user);
    void transaction_history(UserAccount user);
    void logout(UserAccount user);
};