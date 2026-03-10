#include "TransactionRecord.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;




TransactionRecord::TransactionRecord() {}

string TransactionRecord::filename = "../../server/logs/transaction.log"; 

vector<TransactionRecord> TransactionRecord::retrieveTransactions()
{
    ifstream logfile(filename, ios::in);
    vector<TransactionRecord> transactions;
    string line;
    while(getline(logfile, line))
    {
        cout << "logline: " << line;
        TransactionRecord r = deserialize(line);
        transactions.push_back(r);
    }
    return transactions;
}

TransactionRecord TransactionRecord::deserialize(const string &line)
{
    if(line.empty()) return TransactionRecord();

    stringstream strm(line);
    string _timeStamp, _trnxID, _trnxType, _sender, _receiver, _amount;

    if(!getline(strm, _timeStamp, '|')) return TransactionRecord();
    if(!getline(strm, _trnxID, '|')) return TransactionRecord();
    if(!getline(strm, _trnxType, '|')) return TransactionRecord();
    if(!getline(strm, _sender, '|')) return TransactionRecord();
    if(!getline(strm, _receiver, '|')) return TransactionRecord();
    if(!getline(strm, _amount)) return TransactionRecord();

    TransactionRecord r;

    r.timeStamp = _timeStamp;
    r.trnxID = _trnxID;
    r.trnxType = _trnxType;
    r.sender = _sender;
    r.receiver = _receiver;

    try {r.amount = stod(_amount);}
    catch(const exception& e) {cerr <<e.what() <<endl;}

    return r;
}

string TransactionRecord::get_timeStamp() const
{
    return timeStamp;
}
string TransactionRecord::get_trnxID() const
{
    return trnxID;
}
string TransactionRecord::get_trnxType() const
{
    return trnxType;
}
string TransactionRecord::get_sender() const
{
    return sender;
}
string TransactionRecord::get_receiver() const
{
    return receiver;
}
double TransactionRecord::get_amount() const
{
    return amount;
}

void TransactionRecord::set_timeStamp(std::string timeStamp)
{
    this->timeStamp = timeStamp;
}
void TransactionRecord::set_trnxID(std::string trxid)
{
    this->trnxID = trxid;
}
void TransactionRecord::set_trnxType(std::string ttype)
{
    this->trnxType = ttype;
}
void TransactionRecord::set_sender(std::string sender)
{
    this->sender = sender;
}
void TransactionRecord::set_receiver(std::string receiver)
{
    this->receiver = receiver;
}

// int main()
// {
//     // TransactionRecord r;

//     vector<TransactionRecord> transactions = TransactionRecord::retrieveTransactions();




// }