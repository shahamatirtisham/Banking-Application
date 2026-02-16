#include <../shared/classes/UserAccount.hpp>

class UserOperations
{
    public:
    void check_balance();
    void deposit(double amount);
    void withdraw(double amount);
    void transfer_money(UserAccount destAcc, double amount);
    void transaction_history();
    void logout();
};