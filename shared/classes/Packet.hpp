#pragma once
#include "Date.hpp"
#include "UserAccount.hpp"
// #include "UserAccount.hpp"
#include <iostream>
using namespace std;


// class Packet 
// {
// private:
    
//     int clientSock;
//     string command;
//     string name;
//     string username;
//     string password;
//     Date date;
//     double balance;
//     string accountNo;
//     string favAni;
//     string salt;

// public:
//     Packet(int clientSock, string command="", string name="", string username="", string password="", Date date=Date(), 
//         double balance=0, string accountNo="", string favAni="",string salt="");
//     string getCommand() const;
//     string getName() const;
//     string getUsername() const;
//     string getPassword() const;
//     Date getDate() const;
//     double getBalance() const;
//     string getAccountNo() const;
//     string getFavAni() const;
//     string getSalt() const;
//     void write();
//     void read();
//     void display() const;
// };


class Packet 
{
private:
    
    int clientSock;
    string command;
    UserAccount account;
    string salt;

public:
    // Packet(int clientSock, string command="", string name="", string username="", string password="", Date DOB=Date(), 
    //     double balance=0, string accountNo="", string favAni="",string salt="");
    Packet(int clientSock, string command="", UserAccount account=UserAccount(), string salt="");
    string getCommand() const;
    string getName() const;
    string getUsername() const;
    string getPassword() const;
    Date getDate() const;
    double getBalance() const;
    string getAccountNo() const;
    string getFavAni() const;
    string getSalt() const;
    void write();
    void read();
    void display() const;
};