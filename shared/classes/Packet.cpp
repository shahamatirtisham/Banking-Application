#include "Packet.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

Packet::Packet(int clientSock, string command, string name, string username, string password, Date date, 
        double balance, string accountNo, string favAni,string salt)
{
    this->clientSock = clientSock;
    strcpy(this->command, command.c_str());
    strcpy(this->name, name.c_str());
    strcpy(this->username, username.c_str());
    strcpy(this->password, password.c_str());
    this->date = date;
    this->balance = balance;
    strcpy(this->accountNo, accountNo.c_str());
    strcpy(this->favAni, favAni.c_str());
    strcpy(this->salt, salt.c_str());
}

string Packet::getCommand() const
{
    return command;
}
string Packet::getName() const
{
    return name;
}
string Packet::getUsername() const
{
    return username;
}
string Packet::getPassword() const
{
    return password;
}
Date Packet::getDate() const
{
    return date;
}
double Packet::getBalance() const
{
    return balance;
}
string Packet::getAccountNo() const
{
    return accountNo;
}
string Packet::getFavAni() const
{
    return favAni;
}
string Packet::getSalt() const
{
    return salt;
}
void Packet::write()
{
    cout << "writing..\n";
    ::write(clientSock, this->command, 31);
    ::write(clientSock, this->name, 31);
    ::write(clientSock, this->username, 65);
    ::write(clientSock, this->password, 65);
    ::write(clientSock, this->accountNo, 14);
    
    int day = date.getDate();
    int month = date.getMonth();
    int year = date.getYear();
    ::write(clientSock, &day, sizeof(int));
    ::write(clientSock, &month, sizeof(int));
    ::write(clientSock, &year, sizeof(int));

    ::write(clientSock, &balance, sizeof(double));
    ::write(clientSock, favAni, 31);
    ::write(clientSock, salt, 17);

    cout << "writing complete..\n";
}
void Packet:: read()
{ 
    cout << "reading..\n";
    int day;
    int month;
    int year; 

    ::read(clientSock, command, 31);
    ::read(clientSock, name, 31);
    ::read(clientSock, username, 65);
    ::read(clientSock, password, 65);
    ::read(clientSock, accountNo, 14);
    ::read(clientSock, &day, sizeof(int));
    ::read(clientSock, &month, sizeof(int));
    ::read(clientSock, &year, sizeof(int));
    ::read(clientSock, &balance, sizeof(double));
    ::read(clientSock, favAni, 31);
    ::read(clientSock, salt, 17);

    cout << "reading complete..\n";
    
    date = Date(day, month, year);
}

void Packet::display() const
{
    cout << "---------------Displaying Packet---------------\n";
    cout << "Command: " << command << endl;
    cout << "Name: " << name << endl;
    cout << "Username: " << username << endl;
    cout << "Password: " << password << endl;
    cout << "DOB: "; date.display(); cout << endl;
    cout << "Balance: " << balance << endl;
    cout << "Account No: " << accountNo << endl;
    cout << "Favorite animal: " << favAni << endl;
    cout << "Salt: " << salt << endl;
    cout << "-----------------------------------------------\n";
}



