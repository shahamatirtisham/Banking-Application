#pragma once
#include <iostream>

class BankData
{
private:
    long accountsCreated;
    long accountsDeleted;
    void write();
    void read();
public: 
    public: 
    BankData();
    long getAccountsCreated();
    long getAccountsDeleted();
    long getCurrentUsers();
    void createdUser();
    void deletedUser();
    void display();
};

