#include "Packet.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

// Packet::Packet(int clientSock, string command, string name, string username, string password, Date DOB, 
//         double balance, string accountNo, string favAni,string salt) : account(name, username, password,DOB, balance, accountNo, favAni)
// {

//     this->clientSock = clientSock;
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
Packet::Packet(int clientSock, string command, UserAccount account, string salt)
    : account(account)
{
    this->clientSock = clientSock;
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
void Packet::write()
{
    cout << "writing..\n";
    
    ::write(clientSock, this->command.c_str(), 31);
    ::write(clientSock, this->account.getName().c_str(), 31);
    ::write(clientSock, this->account.getUsername().c_str(), 65);
    ::write(clientSock, this->account.getPassword().c_str(), 65);
    ::write(clientSock, this->account.getAccountNo().c_str(), 14); 
    ::write(clientSock, this->account.getFavAni().c_str(), 31);
    ::write(clientSock, this->salt.c_str(), 17);
    
    int day = account.getDOB().getDate();
    int month = account.getDOB().getMonth();
    int year = account.getDOB().getYear();
    ::write(clientSock, &day, sizeof(int));
    ::write(clientSock, &month, sizeof(int));
    ::write(clientSock, &year, sizeof(int));
    double balance = account.getBalance();
    ::write(clientSock, &balance, sizeof(double));
    
    cout << "writing complete..\n";
}



void Packet:: read()
{ 
    cout << "reading..\n";
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
    
    
    ::read(clientSock, _command, 31);
    ::read(clientSock, _name, 31);
    ::read(clientSock, _username, 65);
    ::read(clientSock, _password, 65);
    ::read(clientSock, _accountNo, 14);
    ::read(clientSock, _favAni, 31);
    ::read(clientSock, _salt, 17);
    ::read(clientSock, &day, sizeof(int));
    ::read(clientSock, &month, sizeof(int));
    ::read(clientSock, &year, sizeof(int));
    ::read(clientSock, &_balance, sizeof(double));
    
    
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
    cout << "Command: " << command << endl;
    account.printAccountInfo();
    cout << "Salt: " << salt << endl;
    cout << "-----------------------------------------------------\n";
}



