#include <iostream>
#include "home_page.h"
using namespace std;

HomePage::HomePage(string name, int accountNumber)
{
    this->name = name;
    this->accountNumber = accountNumber;
}

void HomePage::printHeader()
{
    cout <<"====================================================" <<endl;
    cout <<"                 USER DASHBOARD" <<endl;
    cout <<"====================================================" <<endl <<endl;
    cout <<"Welcome, " <<name <<"!" <<endl;
    cout <<"Account Number: " <<accountNumber <<endl <<endl;
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

int HomePage::readChoice()
{
    int choice; 
    while(true)
    {
        cout <<"Enter Choice: "; cin >>choice;
        if(cin.fail() || choice <1 || choice >6)
        {
            cin.clear();
            cin.ignore(9999, '\n');
            cout <<"Invalid Input. Try Again." <<endl <<endl;
        } 
        else return choice;
    }
}

void HomePage::display()
{
    system("clear");
    printHeader();
    printMenu();
}

int HomePage::getChoice()
{
    return readChoice();
}