#include "TransactionRecord.hpp"

bool TransactionRecord::deserialize(const string &line)
{
    if(line.empty()) return false;

    stringstream strm(line);
    string t_amount;

    if(!getline(strm, timeStamp, '|')) return false;
    if(!getline(strm, trnxID, '|')) return false;
    if(!getline(strm, trnxType, '|')) return false;
    if(!getline(strm, sender, '|')) return false;
    if(!getline(strm, receiver, '|')) return false;
    if(!getline(strm, t_amount)) return false;

    try {amount = stod(t_amount);}
    catch(const exception& e) {cerr <<e.what() <<endl;}

    return true;
}

string TransactionRecord::get_timeStamp() 
{
    return timeStamp;
}
string TransactionRecord::get_trnxID() 
{
    return trnxID;
}
string TransactionRecord::get_trnxType() 
{
    return trnxType;
}
string TransactionRecord::get_sender() 
{
    return sender;
}
string TransactionRecord::get_receiver() 
{
    return receiver;
}
double TransactionRecord::get_amount() 
{
    return amount;
}