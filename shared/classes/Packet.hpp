#pragma once
#include "Date.hpp"
#include <iostream>
using namespace std;


class Packet
{
private:
    int clientSock;
    string command;
    string name_seg;
    string username_seg;
    string password_seg;
    Date date_seg;
    double balance_seg;
    string accountNo_seg;
    string favAni_seg;
    string salt_seg;
public:
    Packet(int clientSock, string command="", string name_seg="", string username_seg="", string password_seg="", Date date_seg=Date(), 
        double balance_seg=0, string accountNo_seg="", string favAni_seg="",string salt_seg="");
    string get_command() const;
    string get_name_seg() const;
    string get_username_seg() const;
    string get_password_seg() const;
    Date get_date_seg() const;
    double get_balance_seg() const;
    string get_accountNo_seg() const;
    string get_favAni_seg() const;
    string get_salt_seg() const;
    void write();
    void read();
};
