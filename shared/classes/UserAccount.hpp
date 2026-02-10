#pragma once
#include <iostream>
// #include "Packet.hpp"
// #include <cctype>
#include "Account.hpp"
#include "Date.hpp"
using namespace std;

class Packet;

class UserAccount : public Account
{
private:
    Date DOB;
    double balance;
    string accountNo;
    string favAni;

public:
    UserAccount(string name, string username, string password, Date DOB, double balance, string accountNo, string favAni);
    UserAccount();
    void setDOB(Date DOB=Date());
    void setBalance(float balance=0);
    void setAccountNo(string accounNo);
    void setFavAni(string favAni="");
    Date getDOB() const;
    double getBalance() const;
    string getAccountNo() const;
    string getFavAni() const; 
    void printAccountInfo(bool printAsterisk=1) const;

    UserAccount& operator = (const Packet& packet);
};


bool checkUniqueAccountNo(const string& account_no);
string generateAccountNo();


