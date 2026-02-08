#pragma once
#include <iostream>
// #include <cctype>
#include "Account.hpp"
#include "Date.hpp"
using namespace std;

class UserAccount : public Account
{
private:
    Date DOB;
    double balance;
    string accountNo;
    string favAni;

public:
    UserAccount(string name, string username, string password, Date DOB, double balance, string favAni) ;
    void setDOB(Date DOB);
    void setBalance(float balance=0);
    void setAccountNo();
    void setFavAni(string favAni);
    Date getDOB() const;
    double getBalance() const;
    string getAccountNo() const;
    string getFavAni() const; 
    void printAccountInfo(bool printAsterisk=1) const;

};


