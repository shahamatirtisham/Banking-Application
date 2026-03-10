#include "user-operations.hpp"
#include "../shared/classes/Packet.hpp"
#include "../client/classes/client.hpp"
#include "../../shared/input.hpp"
#include <stdexcept>

// static int sockfd = -1;

// void useroperation_init(int passed_sockfd)
// {
//     if (passed_sockfd >= 0)
//     {
//         sockfd = passed_sockfd;
//         cout << "userOp sockfd: " << sockfd << endl;
//     }
//     else
//     {
//         perror("sockfd not intialized\n");
//     }
// }

// > ▶
// ✔ ✖
// ● ○
// ■ □
// ▲ ▼

UserOperations::UserOperations(int sockfd) : sockfd(sockfd) {}


void UserOperations::check_balance(UserAccount& user)
{
    Packet p("CHECK-BALANCE", user);

    // cout <<"sockfd in user ops: " <<sockfd << endl;

    p.write(sockfd);
    p.read(sockfd);
    if(p.getCommand() != "CURRENT-BALANCE") throw runtime_error("Balance Check Failed!");
    // cout <<"Your Current Balance is : " <<p.getBalance() <<" BDT" <<endl <<endl;
    user.setBalance(p.getBalance());
}

int UserOperations::deposit(UserAccount& user)
{
    Packet p("DEPOSIT", user);

    cout <<"▶ Enter Deposit Amount : ";
    double amount = readDouble();

    if(amount == 0)
    {
        return 0;
    }

    while(amount < 0)
    {
        cout <<"✖ Invalid Amount, Try Again!" <<endl;
        cout <<"Enter Deposit Amount : ";
        amount = readDouble();
    }
    p.setBalance(amount); // ekhane eita balance na, eita deposit amount

    p.write(sockfd);
    p.read(sockfd);

    if(p.getCommand() != "DEPOSIT-SUCCESS") throw runtime_error("Deposit Failed!");
    // cout <<"Deposit Succes! " <<amount <<" BDT was deposited!" <<endl;
    // cout <<"Your New Balance is : " <<p.getBalance() <<" BDT" <<endl <<endl;

    clearScreen();
    return 1;

}

int UserOperations::withdraw(UserAccount& user)
{
    Packet p("WITHDRAW", user);

    cout <<"▶ Enter Withdraw Amount : ";
    double amount = readDouble();

    if(amount == 0)
        return 0;

    while(amount < 0)
    {
        cout <<"✖ Invalid Amount, Try Again!" <<endl;
        cout <<"▶ Enter Withdraw Amount : ";
        amount = readDouble();
    }
    p.setBalance(amount); // ekhane eita balance na, eita withdraw amount

    p.write(sockfd);
    p.read(sockfd);

    if(p.getCommand() == "INSUFFICIENT-BALANCE")
    {
        // cout <<"Insufficient Balance! Withdrawal Failed!" <<endl <<endl;
        clearScreen();
        return -1;
    }
    else if(p.getCommand() == "WITHDRAW-SUCCESS")
    {
        // cout <<"Withdrawal Succes! " <<amount <<" BDT was withdrawn!" <<endl;
        // cout <<"Your New Balance is : " <<p.getBalance() <<" BDT" <<endl <<endl;
        clearScreen();
        return 1;
    }
    else throw runtime_error("Withdraw Failed!");
}

int UserOperations::transfer_money(UserAccount& user)
{
    Packet p("TRANSFER-MONEY", user);
    
    cout <<"▶ Enter Receiver Account No : ";
    string accNo; getline(cin, accNo);
    p.setAccountNo(accNo);  // ekhane eita receiver account no
                            // sender k identify korte username use korbo

    cout <<"▶ Enter Transfer Amount: ";
    double amount = readDouble();

    if(amount == 0)
        return 0;

    while(amount <= 0)
    {
        cout <<"✖ Invalid Amount, Try Again!" <<endl;
        cout <<"▶ Enter Transfer Amount : ";
        amount = readDouble();
    }
    p.setBalance(amount); // ekhane eita balance na, eita transfer amount

    p.write(sockfd);
    p.read(sockfd);

    if(p.getCommand() == "RECEIVER-NOT-FOUND")
    {
        // cout <<"Receiver Not Found! Transfer Failed!" <<endl <<endl;
        clearScreen();
        return -2;
    }
    else if(p.getCommand() == "INSUFFICIENT-BALANCE")
    {
        // cout <<"Insufficient Balance! Transfer Failed!" <<endl <<endl;
        clearScreen();
        return -1;
    }
    else if(p.getCommand() == "TRANSFER-SUCCESS")
    {
        // cout <<"Transfer Succes! BDT " <<amount <<" was Transferred to Account No: " <<accNo <<endl;
        // cout <<"Your New Balance is : " <<p.getBalance() <<" BDT" <<endl <<endl;
        clearScreen();
        return 1;
    }
    else throw runtime_error("Transfer Failed!");
}

void UserOperations::transaction_history(const UserAccount& user)
{
    Packet p("TRANSACTION-HISTORY", user);
    
    p.write(sockfd);
    p.read(sockfd);

    // ekhane kaaj baki ase
}

void UserOperations::logout(const UserAccount& user)
{
    Packet p("LOGOUT", user);

    p.write(sockfd);
}