#include <iostream>
#include "../../shared/classes/UserAccount.hpp"
#include "../../shared/classes/Packet.hpp"
#include "HomePage.hpp"
#include "../shared/input.hpp"
#include "../classes/TextBox.hpp"
#include <stdexcept>
#include <iomanip>
#include <vector>
using namespace std;

HomePage::HomePage(int sockfd, UserAccount user) : sockfd(sockfd), operation(sockfd)
{
    this->user = user;
    
    Packet p("GET-ACC&BAL", user);
    // cout << "writing to get bal\n";
    p.write(this->sockfd);
    // cout << "reading to get bal\n";
    p.read(this->sockfd);
    // cout << "done reading to get bal\n";
    string accNo = p.getAccountNo();
    this->user.setAccountNo(accNo);
    double balance = p.getBalance();
    this->user.setBalance(balance);
    
}

void HomePage::printHeader()
{

    // TextBox tb_homepage("USER DASHBOARD", 40);

    stringstream _balance;
    _balance << fixed << setprecision(2) << user.getBalance();

    vector<string> strs = 
    {
        "USER DASHBOARD"
        "Welcome " + user.getUsername() + "!",
        "Account number: " + user.getAccountNo(),
        "Balance: Tk" + _balance.str()
    };

    TextBox tb_header(strs, 40);

    // cout <<"Welcome, " <<user.getUsername() <<"!" <<endl;
    // cout <<"Account Number: " <<user.getAccountNo() <<endl <<endl;
}

void HomePage::printMenu()
{
    vector<string> strs = 
    {
        "1. Check Balance",
        "2. Deposit",
        "3. Withdraw",
        "4. Transfer Money",
        "5. Transaction History",
        "6. Logout"       
    };

    TextBox tb_options(strs, 40, false, 3);
}

void HomePage::display()
{
    // system("clear");
    printHeader();
    printMenu();
}

int HomePage::getChoice()
{
    int choice; 
    while(true)
    {
        cout <<"▶ Enter Choice: "; choice = readInt();
        if(choice <= 6 && choice >= 1) break;
        else cout <<"Invalid Choice. Try Again." <<endl;
    }
    return choice;
}

void HomePage::userOperation(int choice)
{
    if(choice == 1) 
    {
        try
        {
            operation.check_balance(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 2) 
    {
        try
        {
            operation.deposit(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 3) 
    {
        try
        {
            operation.withdraw(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 4) 
    {
        try
        {
            operation.transfer_money(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 5) 
    {
        try
        {
            operation.transaction_history(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 6) 
    {
        try
        {
            operation.logout(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
}