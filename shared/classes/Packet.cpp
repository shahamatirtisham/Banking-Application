#include "Packet.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

Packet::Packet(int clientSock, string name_seg, string username_seg, string password_seg, Date date_seg, 
        double balance_seg, string accountNo_seg, string favAni_seg,string salt_seg)
{
    this->clientSock = clientSock;
    
    this->name_seg = name_seg;
    this->username_seg = username_seg;
    this->password_seg = password_seg;
    this->date_seg = date_seg;
    this->balance_seg = balance_seg;
    this->accountNo_seg = accountNo_seg;
    this->favAni_seg = favAni_seg;
    this->salt_seg = salt_seg;
}

string Packet::get_name_seg() const
{
    return name_seg;
}
string Packet::get_username_seg() const
{
    return username_seg;
}
string Packet::get_password_seg() const
{
    return password_seg;
}
Date Packet::get_date_seg() const
{
    return date_seg;
}
double Packet::get_balance_seg() const
{
    return balance_seg;
}
string Packet::get_accountNo_seg() const
{
    return accountNo_seg;
}
string Packet::get_favAni_seg() const
{
    return favAni_seg;
}
string Packet::get_salt_seg() const
{
    return salt_seg;
}
void Packet:: write()
{
    ::write(clientSock, name_seg.c_str(), 31);
    ::write(clientSock, username_seg.c_str(), 65);
    ::write(clientSock, password_seg.c_str(), 65);
    
    int day = date_seg.getDate();
    int month = date_seg.getMonth();
    int year = date_seg.getYear();
    ::write(clientSock, &day, sizeof(int));
    ::write(clientSock, &month, sizeof(int));
    ::write(clientSock, &year, sizeof(int));

    ::write(clientSock, &balance_seg, sizeof(double));
    ::write(clientSock, accountNo_seg.c_str(), 14);
    ::write(clientSock, favAni_seg.c_str(), 31);
    ::write(clientSock, salt_seg.c_str(), 17);
}
void Packet:: read()
{ 
    int day;
    int month;
    int year; 

    name_seg.resize(31);
    username_seg.resize(65);
    password_seg.resize(65);
    accountNo_seg.resize(14);
    favAni_seg.resize(31);
    salt_seg.resize(17);

    ::read(clientSock, name_seg.data(), 31);
    ::read(clientSock, username_seg.data(), 65);
    ::read(clientSock, password_seg.data(), 65);
    ::read(clientSock, &day, sizeof(int));
    ::read(clientSock, &month, sizeof(int));
    ::read(clientSock, &year, sizeof(int));
    ::read(clientSock, &balance_seg, sizeof(double));
    ::read(clientSock, accountNo_seg.data(), 14);
    ::read(clientSock, favAni_seg.data(), 31);
    ::read(clientSock, salt_seg.data(), 17);
    
    date_seg = Date(day, month, year);
}



