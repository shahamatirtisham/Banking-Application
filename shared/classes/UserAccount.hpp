#pragma once
#include <iostream>
// #include <cctype>
#include <ctime>
#include "Date.hpp"
#include "input.hpp"
#include "Account.hpp"
#include "clearScreen.hpp"
using namespace std;

class UserAccount : public Account
{
private:
    Date DOB;
    double balance;
    string accountNo;
    string favAni;

public:
    UserAccount(string name, string username, string password, Date DOB, double balance, string favAni) 
        : Account(name, username, password)
    {
        setDOB(DOB);
        setBalance(balance);
        setAccountNo();
        setFavAni(favAni);
    }
    void setDOB(Date DOB);
    void setBalance(float balance);
    void setAccountNo();
    void setFavAni(string favAni);
    Date getDOB() const;
    double getBalance() const;
    string getAccountNo() const;
    string getFavAni() const; 
    void printAccountInfo(bool printAsterik) const;

};


void UserAccount::setDOB(Date DOB)
{
       this->DOB = DOB;
}    

void UserAccount::setBalance(float balance=0)
{
    if(balance >= 0)
        this->balance = balance;
}    
void UserAccount::setAccountNo()
{
    this->accountNo = generateAccountNo();
}
bool checkUniqueAccountNo(const string& account_no)
{
    // input dbms code
    return true;
}
string generateAccountNo()
{
    while(1)
    {
        string accountNo = "";
        srand(time(NULL));
        
        for(int i = 0; i < 13; i++)
        {
            int random_no = rand() % 10;
            accountNo += random_no + '0';
        }
        accountNo[13] = '\0';
    
        if(checkUniqueAccountNo(accountNo) == 1)
        {
            return accountNo;
        }
    }

}    
void UserAccount::setFavAni(string favAni)
{
    this->favAni = favAni;
}  
Date UserAccount::getDOB() const
{
    return DOB;
}
double UserAccount::getBalance() const
{
    return balance;
}
string UserAccount::getAccountNo() const
{
    return accountNo;
}
string UserAccount::getFavAni() const
{
    return favAni;
}
void UserAccount::printAccountInfo(bool printAsterik=0) const
{
    cout << "Name: " << name << endl;
    if(printAsterik)
    {
        cout << "Username: ***\n";
        cout << "Password: ***\n";
    }
    cout << "Date of Birth: ";
    DOB.display(); cout << endl;
    cout << "Balance: " << balance;
    cout << "Account Number: " << accountNo << endl;
    if(printAsterik)
    {
        cout << "Favorite Animal: ***\n";
    }
}


