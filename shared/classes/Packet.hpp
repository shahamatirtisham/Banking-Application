#pragma once
#include "Date.hpp"
// #include "UserAccount.hpp"
#include <iostream>
using namespace std;


class Packet
{
private:
    int clientSock;
    char command[31];
    char name[31];
    char username[65];
    char password[65];
    Date date;
    double balance;
    char accountNo[14];
    char favAni[31];
    char salt[17];
public:
    Packet(int clientSock, string command="", string name="", string username="", string password="", Date date=Date(), 
        double balance=0, string accountNo="", string favAni="",string salt="");
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
