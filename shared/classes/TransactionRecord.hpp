#pragma once
#include <iostream>
#include<sstream>
using namespace std;

class TransactionRecord
{
    string timeStamp, trnxID, trnxType, sender, receiver;
    double amount;
    public:
    TransactionRecord() {}
    bool deserialize(const string &line);
    string get_timeStamp();
    string get_trnxID();
    string get_trnxType(); 
    string get_sender(); 
    string get_receiver();
    double get_amount();

};