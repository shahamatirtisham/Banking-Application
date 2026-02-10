#include <iostream>
#include "Packet.hpp"
#include "UserAccount.hpp"
// #include <cctype>
#include <ctime>
#include "Date.hpp"
#include "input.hpp"
using namespace std;



UserAccount::UserAccount(string name, string username, string password, Date DOB, double balance, string accountNo, string favAni) 
        : Account(name, username, password)
{
    setDOB(DOB);
    setBalance(balance);
    setAccountNo(accountNo);
    setFavAni(favAni);
}

UserAccount::UserAccount() : Account()
{
    setDOB();
    setBalance();
    setAccountNo("");
    setFavAni();
}


void UserAccount::setDOB(Date DOB)
{
       this->DOB = DOB;
}    

void UserAccount::setBalance(float balance)
{
    if(balance >= 0)
        this->balance = balance;
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
void UserAccount::setAccountNo(string accounNo)
{
    if(accountNo == "")
        this->accountNo = generateAccountNo();
    else    
        this->accountNo = accounNo;
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
void UserAccount::printAccountInfo(bool printAsterisk) const
{

    cout << "-----------------Displaying Account------------------\n";
    cout << "Name: " << name << endl;

    if(printAsterisk)
    {
        cout << "Username: ***\n";
        cout << "Password: ***\n";
    }
    else
    {
        cout << "Username: " << username << endl;
        cout << "Password: " << password << endl;
    }
    cout << "Date of Birth: ";
    DOB.display(); cout << endl;
    cout << "Balance: " << balance << endl;
    cout << "Account Number: " << accountNo << endl;
    if(printAsterisk)
    {
        cout << "Favorite Animal: ***\n";
    }
    else
    {
        cout << "Favorite Animal: " << favAni << endl;
    }
    cout << "-----------------------------------------------------\n";
}

UserAccount& UserAccount::operator = (const Packet& packet)
{
    name = packet.getName();
    username = packet.getUsername();
    password = packet.getPassword();
    DOB = packet.getDate();
    balance = packet.getBalance();
    accountNo = packet.getAccountNo();
    favAni = packet.getFavAni();
    return *this;
}


