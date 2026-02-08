#include <iostream>
#include <cctype>
#include "clearScreen.hpp"
using namespace std;

class Account
{
protected:
    string name;
    string username;
    string password;
    
public:
    Account(string name, string username, string password)
    {
        setName(name);
        setUsername(username);
        setPassword(password);   
    }
    void setName(string name);
    void setUsername(string username);
    void setPassword(string password);
    string getUsername() const;
    string getPassword() const;
    string getName() const;
    virtual void printAccountInfo() = 0;
};    



void Account::setName(string name)
{
    this->name = name;
} 
void Account::setUsername(string username)
{
    this->username = username;
}    
void Account::setPassword(string password)
{
      this->password = password;
}  

string Account::getName() const
{
    return name;
}
string Account::getUsername() const
{
    return username;
}
string Account::getPassword() const
{
    return password;
}

