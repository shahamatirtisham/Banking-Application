#pragma once
#include "TransactionRecord.hpp"

class Packet_TR
{
    bool eof;
    TransactionRecord record;
public:
    Packet_TR(const TransactionRecord& record=TransactionRecord(), bool eof=0);
    TransactionRecord getTransactionRecord() const;
    void setEOF(bool eof);
    bool getEOF() const;
    void write(int sockfd);
    void read(int sockfd);
};