#include <iostream>
#include "HomePage.hpp"
#include "../shared/input.hpp"
#include <stdexcept>
using namespace std;

HomePage::HomePage(int sockfd, UserAccount user) : sockfd(sockfd), operation(sockfd)
{
    this->user = user;
}

void HomePage::printHeader()
{
    cout <<"====================================================" <<endl;
    cout <<"                 USER DASHBOARD" <<endl;
    cout <<"====================================================" <<endl <<endl;
    cout <<"Welcome, " <<user.getUsername() <<"!" <<endl;
    cout <<"Account Number: " <<user.getAccountNo() <<endl <<endl;
}

void HomePage::printMenu()
{
    cout <<"1. Check Balance" <<endl;
    cout <<"2. Deposit" <<endl;
    cout <<"3. Withdraw" <<endl;
    cout <<"4. Transfer Money" <<endl;
    cout <<"5. Transaction History" <<endl;
    cout <<"6. Logout" <<endl <<endl;       
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
        cout <<"Enter Choice: "; choice = readInt();
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