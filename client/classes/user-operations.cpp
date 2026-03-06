#include "user-operations.hpp"
#include "../shared/classes/Packet.hpp"
#include "../client/classes/client.hpp"

void UserOperations::check_balance(UserAccount user)
{
    Packet p("CHECK-BALANCE", user);
    p.write(sockfd);
}

void UserOperations::deposit(UserAccount user)
{
    Packet p("DEPOSIT", user);
    p.write(sockfd);
    // ekhane amount er kaj baki ase
}

void UserOperations::withdraw(UserAccount user)
{
    Packet p("WITHDRAW", user);
    p.write(sockfd);
    // ekhane amount er kaj baki ase
}

void UserOperations::transfer_money(UserAccount user)
{
    Packet p("TRANSFER-MONEY", user);
    p.write(sockfd);
    // ekhane receiver account er kaj baki ase
    // ekhane amount er kaj baki ase
}

void UserOperations::transaction_history(UserAccount user)
{
    Packet p("TRANSACTION-HISTORY", user);
    p.write(sockfd);
}

void UserOperations::logout(UserAccount user)
{
    Packet p("LOGOUT", user);
    p.write(sockfd);
}