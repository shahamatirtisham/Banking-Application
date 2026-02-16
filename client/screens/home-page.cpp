#include <iostream>
#include "home-page.hpp"
#include "../shared/input.hpp"
using namespace std;

HomePage::HomePage(UserAccount user)
{
    this->user = user;
}

void HomePage::printHeader()
{
    cout <<"====================================================" <<endl;
    cout <<"                 USER DASHBOARD" <<endl;
    cout <<"====================================================" <<endl <<endl;
    cout <<"Welcome, " <<user.getName() <<"!" <<endl;
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
    system("clear");
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
    if(choice == 1) operation.check_balance(user);
    else if(choice == 2) operation.deposit(user);
    else if(choice == 3) operation.withdraw(user);
    else if(choice == 4) operation.transfer_money(user);
    else if(choice == 5) operation.transaction_history(user);
    else if(choice == 6) operation.logout(user);
}