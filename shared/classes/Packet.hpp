#pragma once
#include "Date.hpp"
#include "Hexadecimal.hpp"
#include "UserAccount.hpp"
// #include "UserAccount.hpp"
#include <iostream>
using namespace std;


class Packet 
{
private:
    Hexadecimal ID;
    string command;
    UserAccount account;
    string salt;

public:
    Packet(string command="", UserAccount account=UserAccount(), string salt="", Hexadecimal ID=Hexadecimal(0));

    void setCommand(string command);
    void setID(Hexadecimal ID);
    void setName(string name);
    void setUsername(string username);
    void setPassword(string password);
    void setDOB(Date DOB);
    void setBalance(double balance);
    void setAccountNo(string accountNo);
    void setFavAni(string favAni);
    void setSalt(string salt);


    string getCommand() const;
    Hexadecimal getID() const;
    string getName() const;
    string getUsername() const;
    string getPassword() const;
    Date getDOB() const;
    double getBalance() const;
    string getAccountNo() const;
    string getFavAni() const;
    string getSalt() const;
    void write(int sockfd);
    void read(int sockfd);
    void display() const;
};