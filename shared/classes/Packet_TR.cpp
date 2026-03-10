#include <iostream>
#include <unistd.h>
#include "Packet_TR.hpp"
using namespace  std;

Packet_TR::Packet_TR(const TransactionRecord& record, bool eof) 
    : eof(eof), record(record) {}


// std::string timeStamp, trnxID, trnxType, sender, receiver;
//     double amount;
// 10-03-2026 14:54:52:479
// T-toffeek-1Un87Xuf4p

TransactionRecord Packet_TR::getTransactionRecord() const
{
    return record;
}

void Packet_TR::setEOF(bool eof)
{
    this->eof = eof;
}
bool Packet_TR::getEOF() const
{   
    return eof;
}

void Packet_TR::write(int sockfd)
{
    // write EOF flag
    ::write(sockfd, &eof, sizeof(bool));

    // write each attribute of TransactionRecord
    ::write(sockfd, record.get_timeStamp().c_str(), 31);
    ::write(sockfd, record.get_trnxID().c_str(), 31);
    ::write(sockfd, record.get_trnxType().c_str(), 31);
    ::write(sockfd, record.get_sender().c_str(), 31);
    ::write(sockfd, record.get_receiver().c_str(), 31);

    double amount = record.get_amount();
    ::write(sockfd, &amount, sizeof(double));
}
void Packet_TR::read(int sockfd)
{
    char _timeStamp[31] = {0};
    char _trnxID[31] = {0};
    char _trnxType[31] = {0};
    char _sender[31] = {0};
    char _receiver[31] = {0};
    double _amount;

    ::read(sockfd, &eof, sizeof(bool));

    ::read(sockfd, _timeStamp, 31);
    ::read(sockfd, _trnxID, 31);
    ::read(sockfd, _trnxType, 31);
    ::read(sockfd, _sender, 31);
    ::read(sockfd, _receiver, 31);
    ::read(sockfd, &_amount, sizeof(double));

    record.set_timeStamp(_timeStamp);
    record.set_trnxID(_trnxID);
    record.set_trnxType(_trnxType);
    record.set_sender(_sender);
    record.set_receiver(_receiver);
    record.set_amount(_amount);
}