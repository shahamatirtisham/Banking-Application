#include <iostream>
#include "bankData.hpp"
using namespace std;


BankData::BankData()
{
    currUserIndex = 0;
    accountsCreated = 0;
    accountDeleted = 0;
    currentUsers = 0;
}
void bankData_init()
{
    FILE *fp = fopen("bankData.bin")
}