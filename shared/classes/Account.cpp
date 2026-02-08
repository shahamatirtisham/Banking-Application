#include "Account.hpp"
// #include <iostream>


// using namespace std;





Account::Account(string name, string username, string password)
{
    setName(name);
    setUsername(username);
    setPassword(password);   
}
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

