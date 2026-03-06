#include "user-operations.hpp"
#include "../shared/classes/Packet.hpp"
#include "../client/classes/client.hpp"
#include "../../shared/input.hpp"

void UserOperations::check_balance(UserAccount user)
{
    Packet p("CHECK-BALANCE", user);

    p.write(sockfd);
    p.read(sockfd);
}

void UserOperations::deposit(UserAccount user)
{
    Packet p("DEPOSIT", user);

    cout <<"Enter Deposit Amount : ";
    double amount = readDouble();
    while(amount <= 0)
    {
        cout <<"Invalid Amount, Try Again!" <<endl;
        cout <<"Enter Deposit Amount : ";
        amount = readDouble();
    }
    p.setBalance(amount); // ekhane eita balance na, eita deposit amount

    p.write(sockfd);
    p.read(sockfd);
}

void UserOperations::withdraw(UserAccount user)
{
    Packet p("WITHDRAW", user);

    cout <<"Enter Withdraw Amount : ";
    double amount = readDouble();
    while(amount <= 0)
    {
        cout <<"Invalid Amount, Try Again!" <<endl;
        cout <<"Enter Withdraw Amount : ";
        amount = readDouble();
    }
    p.setBalance(amount); // ekhane eita balance na, eita withdraw amount

    p.write(sockfd);
    p.read(sockfd);
}

void UserOperations::transfer_money(UserAccount user)
{
    Packet p("TRANSFER-MONEY", user);
    
    cout <<"Enter Receiver Account No : ";
    string accNo; cin >>accNo;
    p.setAccountNo(accNo);  // ekhane eita receiver account no
                            // sender k identify korte username use korbo

    cout <<"Enter Transfer Amount: ";
    double amount = readDouble();
    while(amount <= 0)
    {
        cout <<"Invalid Amount, Try Again!" <<endl;
        cout <<"Enter Transfer Amount : ";
        amount = readDouble();
    }
    p.setBalance(amount); // ekhane eita balance na, eita transfer amount

    p.write(sockfd);
    p.read(sockfd);
}

void UserOperations::transaction_history(UserAccount user)
{
    Packet p("TRANSACTION-HISTORY", user);
    
    p.write(sockfd);
    p.read(sockfd);

    // ekhane kaaj baki ase
}

void UserOperations::logout(UserAccount user)
{
    Packet p("LOGOUT", user);

    p.write(sockfd);
    p.read(sockfd);

    // ekhane kaaj baki ase
}