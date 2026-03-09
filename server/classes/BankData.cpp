#include <iostream>
#include "BankData.hpp"
using namespace std;


BankData::BankData()
{
    FILE *fp = fopen("../server/data/bankData.bin", "rb");
    if(fp == NULL)
    {
        this->accountsCreated = 0;
        this->accountsDeleted = 0;
        cout << "BankData.bin does not exist.Gotta create\n";
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
    FILE* fp = fopen("../server/data/bankData.bin", "wb");
    if(fp == NULL)
    {
        cout << "Failed opening bankData.bin for writing"<< endl;
        exit(0);
    }
    else
    {
        fwrite(this, sizeof(*this), 1, fp);
        fclose(fp);
    }
}
void BankData::read()
{
    FILE *fp =  fopen("../server/data/bankData.bin", "rb");
    if(fp == NULL)
    {
        cout << "Failed opening bankData.bin for reading"<< endl;
        exit(0);
    }
    else
    {
        fread(this, sizeof(*this), 1, fp);
        fclose(fp);
    }
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
    cout << "------------Bank Information------------\n";
    cout << "Total Accounts Created: " << accountsCreated << endl;
    cout << "Total Accounts Deleted: " << accountsDeleted << endl;
    cout << "----------------------------------------\n";
}
