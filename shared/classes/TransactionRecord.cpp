#include "TransactionRecord.hpp"
// #include "../../client/classes/TextBox.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;




TransactionRecord::TransactionRecord() {}

// string TransactionRecord::filename = "../../server/logs/transaction.log"; 
string TransactionRecord::filename = "server/logs/transaction.log"; 

vector<TransactionRecord> TransactionRecord::retrieveTransactions(string username)
{
    ifstream logfile(filename, ios::in);
    vector<TransactionRecord> transactions;
    string line;
    while(getline(logfile, line))
    {
        // cout << "logline: " << line << endl;
        TransactionRecord r = deserialize(line);

        if(r.sender == username || r.receiver == username)
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

    // r.display();

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

void TransactionRecord::set_timeStamp(string timeStamp)
{
    this->timeStamp = timeStamp;
}
void TransactionRecord::set_trnxID(string trxid)
{
    this->trnxID = trxid;
}
void TransactionRecord::set_trnxType(string ttype)
{
    this->trnxType = ttype;
}
void TransactionRecord::set_sender(string sender)
{
    this->sender = sender;
}
void TransactionRecord::set_receiver(string receiver)
{
    this->receiver = receiver;
}
void TransactionRecord::set_amount(double amt)
{
    amount = amt;
}



#include <vector>
#include <sstream>
#include <iomanip>

vector<string> TransactionRecord::getTransactionVector() const
{
    vector<string> lines;

    string date, time;
    size_t pos = timeStamp.find(' ');
    if (pos != string::npos)
    {
        date = timeStamp.substr(0, pos);
        time = timeStamp.substr(pos + 1);
    }
    else
    {
        date = timeStamp;
    }

    string typeFull;
    if (trnxType == "T") typeFull = "Transfer";
    else if (trnxType == "D") typeFull = "Deposit";
    else if (trnxType == "W") typeFull = "Withdraw";
    else typeFull = "Unknown";

    auto format = [](const string& label, const string& value)
    {
        stringstream ss;
        ss << left << setw(18) << label << ": " << value;
        return ss.str();
    };

    lines.push_back(format("Type", typeFull));
    lines.push_back(format("Transaction ID", trnxID));
    lines.push_back(format("Sender", sender));

    if (trnxType == "T")
        lines.push_back(format("Receiver", receiver));

    stringstream amountSS;
    amountSS << fixed << setprecision(2) << amount;
    lines.push_back(format("Amount", amountSS.str()));

    lines.push_back(format("Date", date));
    lines.push_back(format("Time", time));

    return lines;
}

// int main()
// {
//     // TransactionRecord r;

//     vector<TransactionRecord> transactions = TransactionRecord::retrieveTransactions("smelly");

//     for(auto record : transactions)
//     {
//         vector<string> lines = record.getTransactionVector();

//         for(auto line : lines)
//         {
//             cout << line << endl;
//         }
//     }


// }