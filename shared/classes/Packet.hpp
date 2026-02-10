#pragma once
#include "Date.hpp"
#include "UserAccount.hpp"
// #include "UserAccount.hpp"
#include <iostream>
using namespace std;


class Packet 
{
private:
    string command;
    UserAccount account;
    string salt;

public:
    Packet(string command="", UserAccount account=UserAccount(), string salt="");
    string getCommand() const;
    string getName() const;
    string getUsername() const;
    string getPassword() const;
    Date getDate() const;
    double getBalance() const;
    string getAccountNo() const;
    string getFavAni() const;
    string getSalt() const;
    void write(int sockfd);
    void read(int sockfd);
    void display() const;
};