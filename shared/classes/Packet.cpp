#include "Packet.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

// Packet::Packet(int sockfd, string command, string name, string username, string password, Date DOB, 
//         double balance, string accountNo, string favAni,string salt) : account(name, username, password,DOB, balance, accountNo, favAni)
// {

//     this->sockfd = sockfd;
//     this->command = command;
//     account.setName(name);
//     account.setUsername(username);
//     account.setPassword(password);
//     account.setDOB(DOB);
//     account.setBalance(balance);
//     account.setAccountNo(accountNo);
//     account.setFavAni(favAni);
//     this->salt = salt;
// }
Packet::Packet(string command, UserAccount account, string salt)
    : account(account)
{
    this->command = command;
    this->salt = salt;
}
string Packet::getCommand() const
{
    return command;
}
string Packet::getName() const
{
    return account.getName();
}
string Packet::getUsername() const
{
    return account.getUsername();
}
string Packet::getPassword() const
{
    return account.getPassword();
}
Date Packet::getDate() const
{
    return account.getDOB();
}
double Packet::getBalance() const
{
    return account.getBalance();
}
string Packet::getAccountNo() const
{
    return account.getAccountNo();
}
string Packet::getFavAni() const
{
    return account.getFavAni();
}
string Packet::getSalt() const
{
    return salt;
}
void Packet::write(int sockfd)
{
    cout << "writing on sockfd = " << sockfd << endl;
    
    ::write(sockfd, this->command.c_str(), 31);
    ::write(sockfd, this->account.getName().c_str(), 31);
    ::write(sockfd, this->account.getUsername().c_str(), 65);
    ::write(sockfd, this->account.getPassword().c_str(), 65);
    ::write(sockfd, this->account.getAccountNo().c_str(), 14); 
    ::write(sockfd, this->account.getFavAni().c_str(), 31);
    ::write(sockfd, this->salt.c_str(), 17);
    
    int day = account.getDOB().getDate();
    int month = account.getDOB().getMonth();
    int year = account.getDOB().getYear();
    ::write(sockfd, &day, sizeof(int));
    ::write(sockfd, &month, sizeof(int));
    ::write(sockfd, &year, sizeof(int));
    double balance = account.getBalance();
    ::write(sockfd, &balance, sizeof(double));
    
    cout << "writing complete..\n";
}
void Packet:: read(int sockfd)
{ 
    cout << "reading on sockfd = " << sockfd << endl;
    int day;
    int month;
    int year; 
    
    char _command[31] = {0};
    char _name[31] = {0};
    char _username[65] = {0};
    char _password[65] = {0};
    char _accountNo[14] = {0};
    char _favAni[31] = {0};
    char _salt[17] = {0};
    double _balance;
    
    
    ::read(sockfd, _command, 31);
    cout << "read command ";
    ::read(sockfd, _name, 31);
    cout << "read name ";
    ::read(sockfd, _username, 65);
    cout << "read username ";
    ::read(sockfd, _password, 65);
    cout << "read password ";
    ::read(sockfd, _accountNo, 14);
    cout << "read accNO ";
    ::read(sockfd, _favAni, 31);
    cout << "read favAni ";
    ::read(sockfd, _salt, 17);
    cout << "read salt ";
    ::read(sockfd, &day, sizeof(int));
    ::read(sockfd, &month, sizeof(int));
    ::read(sockfd, &year, sizeof(int));
    ::read(sockfd, &_balance, sizeof(double));
    cout << "read data & balance\n";
    
    
    command = _command;
    account.setName(_name);
    account.setUsername(_username);
    account.setPassword(_password);
    account.setAccountNo(_accountNo);
    account.setFavAni(_favAni);
    account.setDOB(Date(day, month, year));
    salt = _salt;
    
    
    
    cout << "reading complete..\n";
    
}

void Packet::display() const
{
    cout << "------------------Displaying Packet------------------\n";
    //cout << "Sockfd: " << sockfd << endl;
    cout << "Command: " << command << endl;
    account.printAccountInfo(0);
    cout << "Salt: " << salt << endl;
    cout << "-----------------------------------------------------\n";
}



