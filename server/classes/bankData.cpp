#include <iostream>
#include "bankData.hpp"
using namespace std;


BankData::BankData()
{
    FILE *fp = fopen("../data/bankData.bin", "rb");
    if(fp == NULL)
    {
        this->accountsCreated = 0;
        this->accountsDeleted = 0;
        
        write();
    }
    else
    {
        fclose(fp);
        read();
    }
}

void BankData::write()
{   
    FILE* fp = fopen("../data/bankData.bin", "wb");
    fwrite(this, sizeof(*this), 1, fp);
    fclose(fp);
}
void BankData::read()
{
    FILE *fp =  fopen("../data/bankData.bin", "rb");
    fread(this, sizeof(*this), 1, fp);
    fclose(fp);
}
long BankData::getAccountsCreated()
{
    read();
    return accountsCreated;
}
long BankData::getAccountsDeleted()
{
    read();
    return accountsDeleted;
}
long BankData::getCurrentUsers()
{
    read();
    return accountsCreated - accountsDeleted;
}

void BankData::createdUser()
{
    read();
    accountsCreated++;
    write();
}
void BankData::deletedUser()
{
    read();
    accountsDeleted++;
    write();
}
void BankData::display()
{
    cout << "------------Bank Information------------";
    cout << "Total Accounts Created: " << accountsCreated << endl;
    cout << "Total Accounts Deleted: " << accountsDeleted << endl;
    cout << "----------------------------------------";
}
